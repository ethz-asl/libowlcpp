/** @file "/owl_cpp/include/owl_cpp/print.hpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef PRINT_HPP_
#define PRINT_HPP_
#include <string>
#include <iostream>
#include <sstream>
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
namespace b = boost;

#include "owl_cpp/rdf/triple_store.hpp"
#include "owl_cpp/rdf/triple.hpp"
#include "owl_cpp/rdf/query_nodes.hpp"
#include "owl_cpp/terms/uri_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp{

/**
*******************************************************************************/
inline void expand(
      const Node_base& nb,
      const Triple_store& store,
      std::ostream& stream
) {
   const std::string& ns = store[nb.ns_id()];
   stream << ns;
   const std::string& v = nb.value_str();
   if( *ns.rbegin() != '/' && ! v.empty() ) stream << '#';
   stream << v;
}

/**
*******************************************************************************/
inline void expand(
      const Node_id nid,
      const Triple_store& store,
      std::ostream& stream
) {
   expand(store[nid], store, stream);
}

/** @return full name string
*******************************************************************************/
template<class T> inline std::string expand(
      const T nb, /**< node ID or node object */
      const Triple_store& store /**< triple store */
) {
   std::stringstream stream;
   expand(nb, store, stream);
   return stream.str();
}

/** Print prefixed name of node
*******************************************************************************/
inline void print_short(
      const Node_id id,
      const Triple_store& store,
      std::ostream& stream,
      const char* str = ""
) {
   const Node_base& node = store[id];
   const Ns_id ns_id = node.ns_id();
   if( ns_id == ot::N_0::id() ) {
      stream << '\"' << node.value_str() << '\"';
   } else {
      stream << store.prefix(ns_id) << ':' << node.value_str();
   }
   stream << str;
}

/** Print RDF triple
*******************************************************************************/
inline void print(
      const Triple& t,
      const Triple_store& store,
      std::ostream& stream,
      const char* str = ""
) {
   stream
   << "Triple {";
   print_short(t.subject(), store, stream, " ");
   print_short(t.predicate(), store, stream, " ");
   print_short(t.object(), store, stream, "}");
   stream << str;
}

/** Print RDF triple
*******************************************************************************/
inline std::string print( const Triple& t, const Triple_store& store) {
   std::stringstream stream;
   stream
   << "Triple {";
   print_short(t.subject(), store, stream, " ");
   print_short(t.predicate(), store, stream, " ");
   print_short(t.object(), store, stream, "}");
   return stream.str();
}

/** Print namespaces
*******************************************************************************/
inline void print_namespaces(const Triple_store& store,std::ostream& stream) {
   BOOST_FOREACH(const Ns_id ns_id, store.namespace_ids()) {
      if( ns_id == ot::N_0::id() || ns_id == ot::N__::id() )
         continue;
      stream << store.prefix(ns_id) << "=\"" << store[ns_id] << "\"\n";
   }
}

/** Print contents of triple store: namespaces and triples
*******************************************************************************/
inline void print(const Triple_store& store, std::ostream& stream) {
   print_namespaces(store, stream);
   BOOST_FOREACH(const Triple& tr, store.triples()) {
      print(tr, store, std::cout, "\n");
   }
}

}//namespace owl_cpp

#endif /* PRINT_HPP_ */
