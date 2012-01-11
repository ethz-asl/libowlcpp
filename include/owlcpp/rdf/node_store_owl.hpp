/** @file "/owlcpp/include/owlcpp/rdf/node_store_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_OWL_HPP_
#define NODE_STORE_OWL_HPP_
#include "owlcpp/rdf/node_store.hpp"
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{

/**@brief Store RDF nodes including standard OWL/RDF nodes
*******************************************************************************/
struct OWLCPP_RDF_DECL Node_store_owl : public Node_store {
   Node_store_owl();
   Node_id insert(Node const& node);
};

}//namespace owlcpp
#endif /* NODE_STORE_OWL_HPP_ */
