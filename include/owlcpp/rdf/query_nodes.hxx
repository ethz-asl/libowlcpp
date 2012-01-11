/** @file "/owlcpp/include/owlcpp/rdf/query_nodes.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef QUERY_NODES_HPP_
#define QUERY_NODES_HPP_
#include "boost/tuple/tuple.hpp"
namespace b = boost;
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{

/**
*******************************************************************************/
inline Node_id insert_node(const std::string& node, Triple_store& store) {
   const b::tuple<std::string,std::string>p = Triple_store::split(node);
   const Ns_id ns_id = store.insert_namespace(p.get<0>());
   return store.insert_node(ns_id, p.get<1>());
}

/**
*******************************************************************************/
OWLCPP_RDF_DECL Node_id find_node(
      const std::string& iri,
      const std::string& term,
      const Triple_store& store
);

/**
*******************************************************************************/
OWLCPP_RDF_DECL bool have_node(
      const std::string& iri_term,
      const Triple_store& store
);

/**
@throw exception if iri or term are not present in store
*******************************************************************************/
inline Node_id find_node(
      const std::string& iri_term,
      const Triple_store& store
) {
   const b::tuple<std::string,std::string> t = Triple_store::split(iri_term);
   return find_node(t.get<0>(), t.get<1>(), store);
}

/** return unique short name
@details Short names are generated and not stored
*******************************************************************************/
inline std::string short_name(const Node_id id, const Triple_store& store) {
   const Node_base& node = store[id];
   const Ns_id ns_id = node.ns_id();
   std::stringstream str;
   str << store.prefix(ns_id) << ':' << node.value_str();
   return str.str();
}

/**@brief Find node ID from its unique short name
@details Since short names are not stored, split it into prefix and term name
and search Triple_store for node ID
*******************************************************************************/
OWLCPP_RDF_DECL Node_id short_name_id(const std::string& short_name, const Triple_store& store);


}//namespace owlcpp

#endif /* QUERY_NODES_HPP_ */
