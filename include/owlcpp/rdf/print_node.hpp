/** @file "/owlcpp/include/owlcpp/rdf/print_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PRINT_NODE_HPP_
#define PRINT_NODE_HPP_
#include <ostream>
#include "boost/lexical_cast.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
template<class Ch, class Tr> inline std::basic_ostream<Ch,Tr>&
   stream(std::basic_ostream<Ch,Tr>& os, Node const& node);//TODO

/**@return node ID string
*******************************************************************************/
inline std::string to_string(const Node_id nid) {
   return "NodeID" + boost::lexical_cast<std::string>(nid());
}

/**@return IRI node string with generated namespace prefix
*******************************************************************************/
std::string to_string(Node_iri const& node) {
   return to_string(node.ns_id()) + ':' + node.name();
}

/**@return IRI node string with complete namespace
*******************************************************************************/
inline std::string to_string_full(Node_iri const& node, Triple_store const& ts) {
   if( node.name().empty() ) return ts[node.ns_id()];
   return ts[node.ns_id()] + '#' + node.name();
}

/**@return IRI node string with namespace prefix
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(Node_iri const&, Triple_store const&);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_iri const&, Triple_store const&);




}//namespace owlcpp
#endif /* PRINT_NODE_HPP_ */
