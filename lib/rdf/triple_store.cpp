/** @file "/owl_cpp/lib/triple_store.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include "owl_cpp/triple_store.hpp"

#include "boost/mpl/for_each.hpp"
namespace bmp = boost::mpl;
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/trim.hpp"
namespace b = boost;

#include "owl_cpp/triple.hpp"
#include "owl_cpp/query_triples.hpp"
#include "owl_cpp/print.hpp"
#include "node_type.hpp"
#include "type_vector.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp {

/*
*******************************************************************************/
Triple_store::Triple_store() {
   enter_standard_uris();
   enter_standard_terms();
}

namespace {
/**
*******************************************************************************/
template<class S> class Store_std_uris {
   mutable S& names_;
   mutable S& prefixes_;
public:
   Store_std_uris(S& names, S& prefixes) : names_(names), prefixes_(prefixes) {}

   template<class T> void operator()(const T&) const {
      assert( ! names_.have(T::id()) );
      assert( ! names_.find(T::name) );
      names_.insert( T::name, T::id() );
      assert( ! prefixes_.have(T::id()) );
      assert( ! prefixes_.find(T::prefix) );
      prefixes_.insert( T::prefix, T::id() );
   }
};

/**
*******************************************************************************/
template<class S> class Store_std_terms {
   mutable S& stor_;
public:
   Store_std_terms(S& stor) : stor_(stor) {}

   template<class T> void operator()(const T&) const {
      stor_.insert( Node_std<T>(), T::id() );
   }
};

}//namespace anonymous

/*
*******************************************************************************/
void Triple_store::enter_standard_uris() {
   Store_std_uris<uri_store_t> ssu(uris_, prefixes_);
   bmp::for_each<ot::mpl_vector_namespaces_all_t>(ssu);
}

/*
*******************************************************************************/
void Triple_store::enter_standard_terms() {
   Store_std_terms<term_store_t> sso(nodes_);
   bmp::for_each<ot::mpl_vector_terms_other_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_rdfs_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_rdf_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_owl1_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_owl2_t>(sso);
}

/**@details IRI comparison is case- and white space- sensitive
*******************************************************************************/
Ns_id Triple_store::insert_namespace(const std::string& iri, const std::string& prefix) {
   const Ns_id id = uris_.insert(iri);
   if( prefix.empty() ) {
      if( ! prefixes_.have(id) ) {
         std::string str("ns");
         str += b::lexical_cast<std::string>(id());
         prefixes_.insert(str, id);
      }
   } else {
      if( prefixes_.have(id) ) {
         if( prefix != prefixes_.get(id) )
            BOOST_THROW_EXCEPTION(
                  Err() << Err::msg_t("re-assigning namespace prefix")
                        << Err::str1_t(iri)
                        << Err::str2_t(prefixes_.get(id))
                        << Err::str3_t(prefix)
            );
      } else {
         uri_store_t::find_range_t r = prefixes_.find(prefix);
         if( r ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("prefix already in use")
               << Err::str1_t(prefix)
               << Err::str2_t( uris_.get( r.front().second ) )
               << Err::str3_t(iri)
         );
         prefixes_.insert(prefix, id);
      }
   }
   return id;
}

/*
*******************************************************************************/
Node_id Triple_store::insert_node(const Ns_id ns_id, const std::string& name) {
   switch ( ns_id() ) {
      case Node_literal::ns_type::index:
         return insert_node( Node_literal(name) );
      case Node_blank::ns_type::index:
         return insert_node( Node_blank(name) );
      default:
         return insert_node( node_generic_t(ns_id, name) );
   }
}

/*
*******************************************************************************/
Triple::id_t Triple_store::insert(
      const Node_id subj_id,
      const Node_id pred_id,
      const Node_id obj_id
) {
   //check for {iri rdf:type owl:Ontology}
   if(
         obj_id == ot::T_owl_Ontology::id() &&
         pred_id == ot::T_rdf_type::id()
   ){
      check_id(subj_id);
   }

   //check for {iri1 owl:versionIRI iri2}
   if(
         pred_id == ot::T_owl_versionIRI::id()
   ){
      check_id(obj_id);
   }

   //select node that will create the triple
   //if predicate is rdf:type the triple is created by object
   //otherwise, triple is created by predicate
   const Node_id main_node_id =
         pred_id() == ot::T_rdf_type::index ? obj_id : pred_id;

   const Node_base& pred = nodes_.get(main_node_id);
   std::auto_ptr<Triple> triple_p = pred.make_triple(subj_id, pred_id, obj_id);
   triples_.push_back(triple_p.get());
   subjects_[subj_id()].push_back(triple_p.get()); //note where subject was mentioned
   return triple_p.release(); //surrender ownership
}

/*
*******************************************************************************/
Triple_store::~Triple_store() {
   BOOST_FOREACH(Triple* tb, triples_) {
      delete tb;
   }
}

/*
*******************************************************************************/
Triple_store::triple_range_t Triple_store::triples() const {
   return b::make_iterator_range(
         triple_iter_t(triples_.begin()),
         triple_iter_t(triples_.end())
   );
}

/*
*******************************************************************************/
Triple_store::triple_range_t Triple_store::subject_in(const Node_id nid) const {
   const triple_store_t& index = subjects_[nid()];
   return b::make_iterator_range(
         triple_iter_t(index.begin()),
         triple_iter_t(index.end())
   );
}

/*
*******************************************************************************/
bool Triple_store::is_standard(const Ns_id id) const {
   switch ( id() ) {
      case ot::N_owl::index:
      case ot::N_rdf::index:
      case ot::N_rdfs::index:
      case ot::N_xsd::index:
         return true;
      default:
         return false;
   }
}

/*
*******************************************************************************/
void Triple_store::check_id(const Node_id nid) const {
   if(
         find_triples(
               nid,
               ot::T_rdf_type::id(),
               ot::T_owl_Ontology::id(),
               *this
         )
   ) BOOST_THROW_EXCEPTION(
         Err()
         << Err::msg_t("duplicate ontology IRI")
         << Err::str1_t(expand(nid, *this))
   );

   if(
         find_triples(
               blank(),
               ot::T_owl_versionIRI::id(),
               nid,
               *this
         )
   ) BOOST_THROW_EXCEPTION(
         Err()
         << Err::msg_t("duplicate ontology version")
         << Err::str1_t(expand(nid, *this))
   );
}

/*
*******************************************************************************/
b::tuple<std::string,std::string> Triple_store::split(const std::string& node) {
   std::size_t ns_end = node.find_last_of('#');
   std::size_t name_start = ns_end + 1;
   if( std::string::npos == ns_end ) {
      if( std::string::npos == (ns_end = node.find_last_of('/')) ) {
         ns_end = node.length();
         name_start = node.length();
      } else {
         name_start = ++ns_end;
      }
   }
   return b::tuple<std::string,std::string> (
         node.substr(0, ns_end),
         b::trim_copy(node.substr(name_start))
   );
}

/*
*******************************************************************************/
}//namespace owl_cpp
