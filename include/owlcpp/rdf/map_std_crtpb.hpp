/** @file "/owlcpp/include/owlcpp/rdf/map_std_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_STD_CRTPB_HPP_
#define MAP_STD_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/concept/assert.hpp"

#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/rdf/store_node_concept.hpp"

namespace owlcpp{

/**Enable interaction between a mutable maps of namespaces and nodes and
immutable map of standard IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_std_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_ns_type map_ns_type;
   typedef typename traits::map_node_type map_node_type;

   map_ns_type const& _map_ns() const {
      BOOST_CONCEPT_ASSERT((Store_node_concept<Super>));
      return static_cast<Super const&>(*this).map_ns_;
   }

   map_ns_type& _map_ns() {
      BOOST_CONCEPT_ASSERT((Store_node_concept<Super>));
      return static_cast<Super&>(*this).map_ns_;
   }

protected:
   Ns_id ns_id_next() const {return map_std_.ns_id_next();}
   Node_id node_id_next() const {return map_std_.node_id_next();}

public:
   template<class Nodes_std> explicit Map_std_crtpb(Nodes_std const& nodes_std)
   : map_std_(Map_std::get(nodes_std))
   {}

   bool valid(const Ns_id nsid) const {
      BOOST_ASSERT(! (map_std_.have(nsid) && _map_ns().have(nsid)) );
      return map_std_.have(nsid) || _map_ns().have(nsid);
   }

private:
   Map_std const& map_std_;
};

}//namespace owlcpp
#endif /* MAP_STD_CRTPB_HPP_ */
