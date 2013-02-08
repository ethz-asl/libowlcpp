/** @file "/owlcpp/include/owlcpp/rdf/print_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PRINT_NODE_HPP_
#define PRINT_NODE_HPP_
#include <ostream>
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/node_fwd.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/print_id.hpp"
#include "owlcpp/terms/term_methods.hpp"

namespace owlcpp{
class Triple_store;

/**@return node string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node const&);

/**@return IRI node string with generated namespace prefix
*******************************************************************************/
inline std::string to_string(Node_iri const& node) {
   return to_string(node.ns_id()) + ':' + node.fragment();
}

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

/**@return node string with complete namespace
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_full(Node const&, Triple_store const&);

/**@return node string with complete namespace
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_full(const Node_id, Triple_store const&);

/**@return IRI node string with complete namespace
*******************************************************************************/
template<class Store> inline std::string
to_string_full(Node_iri const& node, Store const& store) {
   if( node.fragment().empty() ) return store[node.ns_id()].str();
   return store[node.ns_id()].str() + '#' + node.fragment();
}

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(Node const&, Triple_store const&);

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string_pref(const Node_id, Triple_store const&);

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
template<class Store> inline std::string
to_string_pref(Node_iri const& node, Store const& store) {
   const Ns_id nsid = node.ns_id();
   const std::string pref = store.prefix(nsid);
   if( pref.empty() ) return to_string(node);
   return pref + ':' + node.fragment();
}

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(Node const&, Triple_store const&);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(const Node_id, Triple_store const&);

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
template<class Store> inline std::string
to_string(Node_iri const& node, Store const& store) {
   const Ns_id nsid = node.ns_id();
   const std::string pref = store.prefix(nsid);
   if( pref.empty() ) return to_string_full<Store>(node, store);
   return pref + ':' + node.fragment();
}

/**
*******************************************************************************/
template<class Ch, class Tr> inline std::basic_ostream<Ch,Tr>&
   operator<<(std::basic_ostream<Ch,Tr>& os, Node const& node) {
   return os << to_string(node);
}

}//namespace owlcpp
#endif /* PRINT_NODE_HPP_ */
