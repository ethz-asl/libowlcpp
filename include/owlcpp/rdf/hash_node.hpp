/** @file "/owlcpp/include/owlcpp/rdf/hash_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef HASH_NODE_HPP_
#define HASH_NODE_HPP_
#include "boost/functional/hash.hpp"

#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/node_iri.hpp"

namespace owlcpp{

/**
*******************************************************************************/
inline std::size_t hash_value(Node const& node) {return node.hash();}
inline std::size_t hash_value(Node_iri const& node) {return node.hash();}

}//namespace owlcpp
#endif /* HASH_NODE_HPP_ */
