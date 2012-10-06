/** @file "/owlcpp/include/owlcpp/rdf/print_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PRINT_NODE_HPP_
#define PRINT_NODE_HPP_
#include <iosfwd>
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/node_fwd.hpp"

namespace owlcpp{
class Node_id;
class Triple_store;

/**@brief 
*******************************************************************************/
template<class Ch, class Tr> inline std::basic_ostream<Ch,Tr>&
   stream(std::basic_ostream<Ch,Tr>& os, Node const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node const&);

/**@return IRI node string with generated namespace prefix
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_iri const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_blank const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_bool const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_int const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_unsigned const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_double const& node);

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_string const& node);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node const&, Triple_store const&);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(const Node_id, Triple_store const&);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node_iri const&, Triple_store const&);

/**@return node string with complete namespace
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_full(Node const&, Triple_store const&);

/**@return node string with complete namespace
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_full(const Node_id, Triple_store const&);

/**@return IRI node string with complete namespace
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_full(Node_iri const&, Triple_store const&);

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(Node const&, Triple_store const&);

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(const Node_id, Triple_store const&);

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(Node_iri const&, Triple_store const&);

}//namespace owlcpp
#endif /* PRINT_NODE_HPP_ */
