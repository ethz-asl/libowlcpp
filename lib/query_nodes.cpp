/** @file "/owl_cpp/lib/query_nodes.cpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#include "owl_cpp/query_nodes.hpp"
#include "owl_cpp/triple_store.hpp"
#include "node_type.hpp"

namespace owl_cpp {

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
}//namespace owl_cpp
