/** @file "/owlcpp/include/owlcpp/rdf/node_store_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_OWL_HPP_
#define NODE_STORE_OWL_HPP_
#include "owlcpp/rdf/node_store.hpp"
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{ namespace detail{
class Node_tags_inserter;
}

/**@brief Store RDF nodes including standard OWL/RDF nodes
*******************************************************************************/
struct OWLCPP_RDF_DECL Node_store_owl : public Node_store {
   friend class detail::Node_tags_inserter;
   Node_store_owl();
};

}//namespace owlcpp
#endif /* NODE_STORE_OWL_HPP_ */
