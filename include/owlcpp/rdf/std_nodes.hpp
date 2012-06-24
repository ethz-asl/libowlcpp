/** @file "/owlcpp/include/owlcpp/rdf/std_nodes.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STD_NODES_HPP_
#define STD_NODES_HPP_
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{
class Map_node_std;

/**@brief 
*******************************************************************************/
struct Nodes_none {
   void operator()(Map_node_std& map) const {}
};

/**@brief
*******************************************************************************/
struct OWLCPP_RDF_DECL Nodes_owl {
   void operator()(Map_node_std& map) const;
};


}//namespace owlcpp
#endif /* STD_NODES_HPP_ */
