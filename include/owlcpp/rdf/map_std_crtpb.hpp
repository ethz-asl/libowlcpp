/** @file "/owlcpp/include/owlcpp/rdf/map_std_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_STD_CRTPB_HPP_
#define MAP_STD_CRTPB_HPP_

#include "owlcpp/rdf/map_std.hpp"

namespace owlcpp{

/**Enable interaction between a mutable maps of namespaces and nodes and
immutable map of standard IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_std_crtpb {
protected:
   Ns_id ns_id_next() const {return map_std_.ns_id_next();}
   Node_id node_id_next() const {return map_std_.node_id_next();}

public:
   template<class Nodes_std> explicit Map_std_crtpb(Nodes_std const& nodes_std)
   : map_std_(Map_std::get(nodes_std))
   {}

private:
   Map_std const& map_std_;
};

}//namespace owlcpp
#endif /* MAP_STD_CRTPB_HPP_ */
