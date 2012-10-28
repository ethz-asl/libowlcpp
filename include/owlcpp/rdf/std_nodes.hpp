/** @file "/owlcpp/include/owlcpp/rdf/std_nodes.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STD_NODES_HPP_
#define STD_NODES_HPP_
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{
class Iri_map_base;
class Node_map_base;

/**@brief 
*******************************************************************************/
struct Nodes_none {
   void operator()(Iri_map_base& map) const {}
   void operator()(Node_map_base& map) const {}
};

/**@brief
*******************************************************************************/
struct OWLCPP_RDF_DECL Nodes_owl {
   void operator()(Iri_map_base& map) const;
   void operator()(Node_map_base& map) const;
};


}//namespace owlcpp
#endif /* STD_NODES_HPP_ */