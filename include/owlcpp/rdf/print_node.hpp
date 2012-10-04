/** @file "/owlcpp/include/owlcpp/rdf/print_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PRINT_NODE_HPP_
#define PRINT_NODE_HPP_
#include <iosfwd>
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{
class Node;
class Node_id;
class Node_iri;
class Triple_store;

/**@brief 
*******************************************************************************/
template<class Ch, class Tr> inline std::basic_ostream<Ch,Tr>&
   stream(std::basic_ostream<Ch,Tr>& os, Node const& node);

/**@return node ID string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(const Node_id nid);

/**@return IRI node string with generated namespace prefix
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_iri const& node);

/**@return IRI node string with complete namespace
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_full(Node_iri const& node, Triple_store const& ts);

/**@return IRI node string with namespace prefix
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(Node_iri const&, Triple_store const&);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_iri const&, Triple_store const&);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node const&);

}//namespace owlcpp
#endif /* PRINT_NODE_HPP_ */
