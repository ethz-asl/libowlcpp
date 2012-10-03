/** @file "/owlcpp/lib/rdf/print_node.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/print_node.hpp"

#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::string to_string(const Node_id nid) {
   return "NodeID" + boost::lexical_cast<std::string>(nid());
}

/*
*******************************************************************************/
std::string to_string_full(Node_iri const& node, Triple_store const& ts) {
   if( node.name().empty() ) return ts[node.ns_id()];
   return ts[node.ns_id()] + '#' + node.name();
}


}//namespace owlcpp
