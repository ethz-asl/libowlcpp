/** @file "/owlcpp/lib/query_nodes.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif

#include "owlcpp/rdf/query_nodes.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "../node_type.hpp"

namespace owlcpp {

/*
*******************************************************************************/
Node_id find_node(
      const std::string& iri,
      const std::string& term,
      const Triple_store& store
) {
   const Ns_id ns_id = store[iri];
   return store[ node_generic_t(ns_id, term) ];
}

/*
*******************************************************************************/
bool have_node(
      const std::string& iri_term,
      const Triple_store& store
) {
   const b::tuple<std::string,std::string> t = Triple_store::split(iri_term);
   if( ! store.find(t.get<0>()) ) return false;
   const Ns_id ns_id = store[t.get<0>()];
   return store.find( node_generic_t(ns_id, t.get<1>()) );
}

/*
*******************************************************************************/
Node_id short_name_id(const std::string& short_name, const Triple_store& store) {
   const std::size_t pref_end = short_name.find_first_of(':');
   if( std::string::npos == pref_end )
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("invalid short name")
            << base_exception::str1_t(short_name)
      );
   const Ns_id ns_id = store.prefix_id(short_name.substr(0, pref_end));
   return store[ node_generic_t(ns_id, short_name.substr(pref_end+1)) ];
}

/*
*******************************************************************************/
}//namespace owlcpp
