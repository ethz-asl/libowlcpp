/** @file "/owlcpp/lib/io/parse_to_triple_store.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif

#include "owlcpp/io/parse_to_triple_store.hpp"

#include <set>
#include <cassert>
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem.hpp"
namespace bf = boost::filesystem;
#include "boost/algorithm/string/predicate.hpp"
namespace ba = boost::algorithm;

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "owlcpp/terms/term_tags.hpp"
#include "owlcpp/print.hpp"
namespace ot = owlcpp::terms;
#include "parse.hpp"

namespace owlcpp{

namespace{
typedef std::set<std::string> owl_set_t;

/** Insert raptor-style triples into Triple_store
*******************************************************************************/
class Triple_store_adaptor {
   Triple_store_adaptor();
public:
   Triple_store_adaptor(Triple_store& store)
   : store_(store), imports_() {}

   void insert(const triple_t& triple) {
      const Node_id pred_id = insert(triple.get<1>());
      const Node_id obj_id  = insert(triple.get<2>());

      //check for {iri1, owl:imports, iri2}
      if( pred_id() == ot::T_owl_imports::index ) {
         imports_.insert(triple.get<2>().value);
         return;
      }

      const Node_id subj_id = insert(triple.get<0>());
      store_.insert(subj_id, pred_id, obj_id);
   }

   bool stop_parsing() const {return false;}
   const owl_set_t& imports() const {return imports_;}

private:
   Triple_store& store_;
   owl_set_t imports_;

   Node_id insert(const Term& term) {
      switch (term.type) {
      case Resource: {
         const b::tuple<std::string,std::string> p = Triple_store::split(term.value);
         const Ns_id ns_id = store_.insert_namespace(p.get<0>());
         return store_.insert_node(ns_id, p.get<1>());
      }
      case Literal:
         return store_.insert_node(ot::N_empty::id(), term.value);
      case Anonymous:
         return store_.insert_node(ot::N_blank::id(), term.value);
      default:
         BOOST_THROW_EXCEPTION(
               Parse_err() << Parse_err::msg_t("unknown term type")
         );
      }
   }
};
}//namespace anonymous

/*
*******************************************************************************/
void load(
      std::istream& stream,
      Triple_store& store
) {
   const unsigned char base_iri[] = "default_base_IRI";
   Rdf_parser parser = Rdf_parser::rdfxml(base_iri);
   Triple_store_adaptor tsa(store);
   const unsigned n = num_ontologies(store);
   parser(stream, tsa, n);
}

/*
*******************************************************************************/
void load(
      const std::string& file,
      Triple_store& store
) {
   bf::ifstream ifs(file);
   try{
      load(ifs, store);
   } catch(...) {
      BOOST_THROW_EXCEPTION(
            Parse_err()
            << Parse_err::msg_t("parsing error")
            << Parse_err::str1_t(file)
            << Parse_err::nested_t(b::current_exception())
      );
   }
}

namespace{

/*
*******************************************************************************/
class Iri_finder {
public:
   Iri_finder() : iri_(), version_(), stop_parsing_(false) {}

   bool stop_parsing() const {return stop_parsing_;}

   void insert(const triple_t& triple) {
      if(
            triple.get<2>().type == Resource &&
            triple.get<2>().value == owl_Ontology() &&
            triple.get<1>().type == Resource &&
            triple.get<1>().value == rdf_type()
      ) {
         assert( triple.get<0>().type == Resource );
         iri_ = triple.get<0>().value;
      } else if(
            triple.get<1>().type == Resource &&
            triple.get<1>().value == owl_versionIRI()
      ) {
         assert( triple.get<0>().type == Resource );
         if( triple.get<0>().value != iri_ ) BOOST_THROW_EXCEPTION(
               Parse_err()
               << Parse_err::msg_t("ontology IRI mismatch")
               << Parse_err::str1_t(triple.get<0>().value)
               << Parse_err::str2_t(iri_)
            );
         version_ = triple.get<2>().value;
         stop_parsing_ = true;
      } else if( ! iri_.empty() && triple.get<0>().value != iri_ ) {
         //Most ontologies have no version IRI.
         //To avoid parsing entire ontology searching for version IRI
         //take a short-cut.
         //Assume there is only one owl:Ontology element.
         //If iri_ is found and the subject of current triple is not iri_,
         //we must be outside of owl:Ontology element and may stop searching for version.
         //TODO: Make sure this is actually a good idea.
         stop_parsing_ = true;
      }
   }

   const std::string& iri() const {return iri_;}
   const std::string& version() const {return version_;}

private:
   std::string iri_;
   std::string version_;
   bool stop_parsing_;

   static const std::string& owl_Ontology() {
      static const std::string s = full_name(ot::T_owl_Ontology());
      return s;
   }
   static const std::string& rdf_type() {
      static const std::string s = full_name(ot::T_rdf_type());
      return s;
   }
   static const std::string& owl_versionIRI() {
      static const std::string s = full_name(ot::T_owl_versionIRI());
      return s;
   }
};
}//namespace anonymous

/** @details
*******************************************************************************/
b::tuple<std::string,std::string> ontology_id(const std::string& file) {
   bf::ifstream stream(file);
   const unsigned char base_iri[] = "IRI not found";
   Rdf_parser parser = Rdf_parser::rdfxml(base_iri);
   Iri_finder irif;
   try{
      parser(stream, irif, 0);
   } catch(...) {
      BOOST_THROW_EXCEPTION(
            Parse_err()
            << Parse_err::msg_t("error parsing")
            << Parse_err::str1_t(file)
            << Parse_err::nested_t(b::current_exception())
      );
   }
   return b::make_tuple(irif.iri(), irif.version());
}


/** @details
*******************************************************************************/
void find_ontologies(Catalog& cat, const std::string& path) {
   bf::path p(system_complete(bf::path(path)));
   if( ! exists(p) || is_other(p) ) return;
   if( is_directory(p) ) {
      bf::directory_iterator end_i;
      for( bf::directory_iterator i(p); i != end_i; ++ i ) {
         find_ontologies(cat, i->path().string());
      }
   } else if( ba::iequals(extension(p), ".owl") ) {
      const b::tuple<std::string,std::string> ts = ontology_id(p.string());
      if( ! ts.get<0>().empty() )
         cat.insert(p.string(), ts.get<0>(), ts.get<1>());
   }
}

namespace detail{
/*
*******************************************************************************/
void load_iri(
      const std::string& iri,
      Triple_store& store,
      const Catalog& cat
) {
   if( have_ontology_id(iri, store) ) return;
   const std::string& path = cat.find_location(iri);
   Triple_store_adaptor tsa(store);
   try{
      bf::ifstream ifs(path);
      Rdf_parser parser = Rdf_parser::rdfxml((const unsigned char*)iri.c_str());
      parser(ifs, tsa, num_ontologies(store));
      try{
         BOOST_FOREACH(const std::string& import, tsa.imports() ) {
            load_iri(import, store, cat);
         }
      } catch(...) {
         BOOST_THROW_EXCEPTION(
               Parse_err()
               << Parse_err::msg_t("import error")
               << Parse_err::nested_t(b::current_exception())
         );
      }
   } catch(...) {
      BOOST_THROW_EXCEPTION(
            Parse_err()
            << Parse_err::msg_t("error parsing")
            << Parse_err::str1_t(path)
            << Parse_err::str2_t(iri)
            << Parse_err::nested_t(b::current_exception())
      );
   }
}
}//namespace detail


}//namespace owlcpp
