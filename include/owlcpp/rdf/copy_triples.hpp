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
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace detail{
template<class Src, class Dest> class Copy_adaptor{
   typedef boost::unordered_map<Node_id,Node_id> node_map_t;
   typedef boost::unordered_map<Doc_id,Doc_id> doc_map_t;
   typedef boost::unordered_map<Ns_id,Ns_id> ns_map_t;
public:
   Copy_adaptor(Src const& src, Dest& dest)
   : src_(src), dest_(dest)
   {}

   void operator()(Triple const& t) {
      const Node_id subj = (*this)(t.subject());
      const Node_id pred = (*this)(t.predicate());
      const Node_id obj = (*this)(t.object());
      const Doc_id doc = (*this)(t.document());
      dest_.insert_triple(subj, pred, obj, doc);
   }

   Node_id operator()(const Node_id nid0) {
      const node_map_t::const_iterator i = nm_.find(nid0);
      if( i != nm_.end() ) return i->second;
      Node const& node = src_[nid0];
      if( is_blank(node.ns_id()) ) return insert_blank(nid0, node.value_str());
      if( is_empty(node.ns_id()) ) return insert_literal(nid0, node.value_str());
      return insert_iri(nid0, node);
   }

   Doc_id operator()(const Doc_id did0) {
      const doc_map_t::const_iterator i = dm_.find(did0);
      if( i != dm_.end() ) return i->second;
      const Doc_id did1 = dest_.insert_doc(
               src_.path(did0),
               src_.ontology_iri(did0),
               src_.version_iri(did0)
      ).first;
      dm_.emplace(did0, did1);
      return did1;
   }

   Ns_id operator()(const Ns_id nsid0) {
      const ns_map_t::const_iterator i = nsm_.find(nsid0);
      if( i != nsm_.end() ) return i->second;
      const Ns_id nsid1 = dest_.iris().insert(src_[nsid0]);
      dest_.iris().insert_prefix(nsid1, src_.iris().prefix(nsid0));
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

      const Node_id nid1 = dest_.insert_blank_node(
               (*this)(src_.nodes().blank_node_doc(nid0)),
               name
      );
      nm_.emplace(nid0, nid1);
      return nid1;
   }

   Node_id insert_iri(const Node_id nid0, Node const& node) {
      const Node_id nid1 = dest_.nodes().insert_iri(
               (*this)(node.ns_id()),
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
      const Node_id nid1 = dest_.nodes().insert_literal(
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
         Range r,
         Src const& src,
         Dest& dest
) {
   detail::Copy_adaptor<Src, Dest> ca(src, dest);
   BOOST_FOREACH(Triple const& t, r) ca(t);
}

/**@brief copy all triples from one store to another
@details IRIs, nodes, document info-s are also copied
@param ts1 source of triples
@param ts2 destination
@throw base_exception if @b ts1 contains document already present in @b ts2
*******************************************************************************/
template<class Src, class Dest> inline void copy_triples(Src const& src, Dest& dest) {
   copy_triples(src.triples(), src, dest);
}

}//namespace owlcpp
#endif /* COPY_TRIPLES_HPP_ */
