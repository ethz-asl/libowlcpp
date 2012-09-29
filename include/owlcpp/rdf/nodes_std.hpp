/** @file "/owlcpp/include/owlcpp/rdf/nodes_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODES_STD_HPP_
#define NODES_STD_HPP_
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{
class Map_std;

/**@brief
*******************************************************************************/
struct Nodes_none {
   void operator()(Map_std&) const {}
};

/**@brief
*******************************************************************************/
struct OWLCPP_RDF_DECL Nodes_owl {
   void operator()(Map_std&) const;
};

}//namespace owlcpp
#endif /* NODES_STD_HPP_ */
