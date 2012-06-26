/** @file "/owlcpp/include/owlcpp/rdf/map_ns_std_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NS_STD_CRTPB_HPP_
#define MAP_NS_STD_CRTPB_HPP_

#include "owlcpp/ns_id.hpp"

namespace owlcpp{

/** Enable interaction between a map of user namespaces and
a map of standard namespaces.
Base for curiously recursive template pattern.
*******************************************************************************/
template<class Super> class Map_ns_std_crtpb {

   typedef typename Super::map_ns map_ns_t;

   map_ns_t const& map_ns() const {
      return static_cast<Super const&>(*this).map_ns();
   }

public:
   typedef Super self_type;

   std::string operator[](const Ns_id id) const {

   }

};

}//namespace owlcpp
#endif /* MAP_NS_STD_CRTPB_HPP_ */
