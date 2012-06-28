/** @file "/owlcpp/include/owlcpp/rdf/crtpb_ns_node_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CRTPB_NS_NODE_IRI_HPP_
#define CRTPB_NS_NODE_IRI_HPP_

namespace owlcpp{

/**Enable interaction between a map of namespaces and a map of nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Crtpb_ns_node_iri {
   typedef typename Map_traits<Super>::map_ns_t map_ns_t;
   typedef typename Map_traits<Super>::map_node_t map_node_t;

   map_ns_t const& ns() const {
      return static_cast<Super const&>(*this).namespaces();
   }

   map_ns_t& ns() {
      return static_cast<Super&>(*this).ns_;
   }

   map_node_t const& nodes() const {
      return static_cast<Super const&>(*this).nodes();
   }

   map_node_t& nodes() {
      return static_cast<Super&>(*this).node_;
   }

public:

};


}//namespace owlcpp
#endif /* CRTPB_NS_NODE_IRI_HPP_ */
