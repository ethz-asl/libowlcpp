/** @file "/owlcpp/include/owlcpp/rdf/copy_triples.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef COPY_TRIPLES_HPP_
#define COPY_TRIPLES_HPP_
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include "boost/assert.hpp"

#include "owlcpp/rdf/triple.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/visitor_node.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace detail{

template<class Src, class Dest> class Node_copier : public Visitor_node {
   typedef boost::unordered_map<Node_id,Node_id> node_map_t;
   typedef node_map_t::const_iterator node_iter_t;
   typedef boost::unordered_map<Doc_id,Doc_id> doc_map_t;
   typedef doc_map_t::const_iterator doc_iter_t;
   typedef boost::unordered_map<Ns_id,Ns_id> ns_map_t;
   typedef ns_map_t::const_iterator ns_iter_t;

   typedef detail::Map_traits<Src> traits;
   typedef typename traits::doc_type doc_type;

public:
   Node_copier(Src const& src, Dest& dest)
   : src_(src), dest_(dest)
   {}

   void operator()(Triple const& t) {
      const Node_id subj = cp(t.subj_);
      const Node_id pred = cp(t.pred_);
      const Node_id obj  = cp(t.obj_);
      const  Doc_id doc  = cp(t.doc_);
      dest_.insert(Triple::make(subj, pred, obj, doc));
   }

   Node_id cp(const Node_id nid0) {
      const node_iter_t i = nm_.find(nid0);
      if( i != nm_.end() ) return i->second;
      Node const& node = src_[nid0];
      node.accept(*this);
      nm_.emplace(nid0, last_inserted_id_);
      return last_inserted_id_;
   }

   Doc_id cp(const Doc_id did0) {
      const doc_iter_t i = dm_.find(did0);
      if( i != dm_.end() ) return i->second;
      doc_type const& doc = src_[did0];
      const Node_id iri_id = cp(doc.ontology_iri);
      const Node_id vers_id = cp(doc.version_iri);
      const std::pair<Doc_id, bool> p =
               dest_.insert_doc(
                        iri_id,
                        doc.path,
                        vers_id
               );

      //Assertion is violated if same Doc info has been inserted into destination
      //before %this was created.
      //For example, this will be violated if a document is copied in multiple
      //operations, with multiple instances of %Node_copier
      //BOOST_ASSERT(p.second);

      dm_.emplace(did0, p.first);
      return p.first;
   }

   Ns_id cp(const Ns_id nsid0) {
      const ns_iter_t i = nsm_.find(nsid0);
      if( i != nsm_.end() ) return i->second;
      const Ns_id nsid1 = dest_.insert(src_[nsid0]);
      std::string const& pref = src_.prefix(nsid0);
      if(
               ! pref.empty() &&
                 dest_.prefix(nsid1).empty() &&
               ! dest_.find_prefix(pref)
      ) {
         dest_.insert_prefix(nsid1, pref);
      }
      nsm_.emplace(nsid0, nsid1);
      return nsid1;
   }

private:
   Src const& src_;
   Dest& dest_;
   node_map_t nm_;
   doc_map_t dm_;
   ns_map_t nsm_;
   Node_id last_inserted_id_; /**< return value from visit_impl methods */

   void visit_impl(Node_iri const& node) {
      const Ns_id nsid = cp(node.ns_id());
      last_inserted_id_ = dest_.insert_node_iri(nsid, node.fragment());
   }

   void visit_impl(Node_blank const& node) {
      const Doc_id did = cp(node.document());

      //check if blank node is already present
      //see comment in %cp(const Doc_id)
      //BOOST_ASSERT( ! dest_.find_blank(node.index(), did));

      last_inserted_id_ = dest_.insert_blank(node.index(), did);
   }

   void visit_impl(Node_bool const& node) {
      const Node_id dt = cp(node.datatype());
      last_inserted_id_ = dest_.insert_literal(node.value_str(), dt);
   }

   void visit_impl(Node_int const& node) {
      const Node_id dt = cp(node.datatype());
      last_inserted_id_ = dest_.insert_literal(node.value_str(), dt);
   }

   void visit_impl(Node_unsigned const& node) {
      const Node_id dt = cp(node.datatype());
      last_inserted_id_ = dest_.insert_literal(node.value_str(), dt);
   }

   void visit_impl(Node_double const& node) {
      const Node_id dt = cp(node.datatype());
      last_inserted_id_ = dest_.insert_literal(node.value_str(), dt);
   }

   void visit_impl(Node_string const& node) {
      const Node_id dt = cp(node.datatype());
      last_inserted_id_ = dest_.insert_literal(
               node.value(), dt, node.language()
      );
   }
};

}//namespace detail

/**@brief copy a range of triples from one store to another
@details IRIs, nodes, document info-s are also copied
@param r source range of triples
@param src source triple store
@param dest destination triple store
@throw base_exception if @b ts1 contains document already present in @b ts2
*******************************************************************************/
template<class Range, class Src, class Dest> inline void copy_triples(
         Range const& r,
         Src const& src,
         Dest& dest
) {
   detail::Node_copier<Src, Dest> copier(src, dest);
   BOOST_FOREACH(Triple const& t, r) copier(t);
}

/**@brief copy all triples from one store to another
@details IRIs, nodes, document info-s are also copied
@param src source of triples
@param dest destination
@throw base_exception if @b ts1 contains document already present in @b ts2
*******************************************************************************/
template<class Src, class Dest> inline void copy_triples(Src const& src, Dest& dest) {
   copy_triples(src.map_triple(), src, dest);
}

}//namespace owlcpp
#endif /* COPY_TRIPLES_HPP_ */
