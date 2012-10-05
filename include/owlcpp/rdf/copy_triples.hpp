/** @file "/owlcpp/include/owlcpp/rdf/copy_triples.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef COPY_TRIPLES_HPP_
#define COPY_TRIPLES_HPP_
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"

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

   void operator()(Triple const& tr) {
      const Node_id subj = cp(t.subject());
      const Node_id pred = cp(t.predicate());
      const Node_id obj  = cp(t.object());
      const  Doc_id doc  = cp(t.document());
      dest_.insert_triple(subj, pred, obj, doc);
   }

   Node_id cp(const Node_id nid0) {
      const node_iter_t i = nm_.find(nid0);
      if( i != nm_.end() ) return i->second;
      Node const& node = src_[nid0];
      node.accept(*this);
   }

   Doc_id cp(const Doc_id did0) {
      const doc_iter_t i = dm_.find(did0);
      if( i != dm_.end() ) return i->second;
      Node const& node = src_[did0];

   }

   Ns_id cp(const Ns_id nsid0) {
      const node_iter_t i = nm_.find(nid0);
      if( i != nm_.end() ) return i->second;
      Node const& node = src_[nid0];

   }

private:
   Src const& src_;
   Dest& dest_;
   node_map_t nm_;
   doc_map_t dm_;
   ns_map_t nsm_;

   void visit_impl(Node_iri const& node) {

   }

   void visit_impl(Node_blank const& node) {

   }

   void visit_impl(Node_bool const& node) {

   }

   void visit_impl(Node_int const& node) {

   }

   void visit_impl(Node_unsigned const& node) {

   }

   void visit_impl(Node_double const& node) {

   }

   void visit_impl(Node_string const& node) {

   }
};


template<class Src, class Dest> class Copy_adaptor{
   typedef boost::unordered_map<Node_id,Node_id> node_map_t;
   typedef boost::unordered_map<Doc_id,Doc_id> doc_map_t;
   typedef boost::unordered_map<Ns_id,Ns_id> ns_map_t;
public:
   Copy_adaptor(Src const& src, Dest& dest)
   : src_(src), dest_(dest)
   {}

   void operator()(Triple const& t) {
      const Node_id subj = insert(t.subject());
      const Node_id pred = insert(t.predicate());
      const Node_id obj = insert(t.object());
      const Doc_id doc = insert(t.document());
      dest_.insert_triple(subj, pred, obj, doc);
   }

   Node_id insert(const Node_id nid0) {
      const node_map_t::const_iterator i = nm_.find(nid0);
      if( i != nm_.end() ) return i->second;
      Node const& node = src_[nid0];
      if( is_blank(node.ns_id()) ) return insert_blank(nid0, node.value_str());
      if( is_empty(node.ns_id()) ) return insert_literal(nid0, node.value_str());
      return insert_iri(nid0, node);
   }

   Doc_id insert(const Doc_id did0) {
      const doc_map_t::const_iterator i = dm_.find(did0);
      if( i != dm_.end() ) return i->second;
      const std::pair<Doc_id,bool> p = dest_.insert_doc(
               insert(src_[did0].ontology_iri()),
               src_[did0].path(),
               insert(src_[did0].version_iri())
      );
      BOOST_ASSERT(p.second);
      dm_.emplace(did0, p.first);
      return p.first;
   }

   Ns_id insert(const Ns_id nsid0) {
      const ns_map_t::const_iterator i = nsm_.find(nsid0);
      if( i != nsm_.end() ) return i->second;
      const Ns_id nsid1 = dest_.insert_ns(src_[nsid0]);
      dest_.insert_prefix(nsid1, src_.prefix(nsid0));
      nsm_.emplace(nsid0, nsid1);
      return nsid1;
   }

private:
   Src const& src_;
   Dest& dest_;
   node_map_t nm_;
   doc_map_t dm_;
   ns_map_t nsm_;

   Node_id insert_blank(
            const Node_id nid0,
            std::string const& name
   ) {

      const Node_id nid1 = dest_.insert_blank(
               insert(src_.nodes().blank_node_doc(nid0)),
               name
      );
      nm_.emplace(nid0, nid1);
      return nid1;
   }

   Node_id insert_iri(const Node_id nid0, Node const& node) {
      const Node_id nid1 = dest_.insert_node_iri(
               insert(node.ns_id()),
               node.value_str()
      );
      nm_.emplace(nid0, nid1);
      return nid1;
   }

   Node_id insert_iri(const Node_id nid0) {return insert_iri(nid0, src_[nid0]);}

   Node_id insert_literal(
            const Node_id nid0,
            std::string const& val
   ) {
      const Node_id nid1 = dest_.insert_literal(
               val,
               insert_iri( src_.nodes().datatype(nid0) ),
               src_.nodes().language(nid0)
      );
      nm_.emplace(nid0, nid1);
      return nid1;
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
