/** @file "/owlcpp/lib/rdf/query_node.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/terms/iri_tags.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::string to_string(const Node_id nid) {
   return boost::lexical_cast<std::string>(nid());
}

/*
*******************************************************************************/
std::string to_string(const Node_id nid, Triple_store const& ts) {
   if( is_empty(nid) ) return "";
   Node const& node = ts[nid];
   const std::string name = node.value_str();
   if( is_blank(node.ns_id()) ) return "_:" + name;
   if( is_empty(node.ns_id()) ) {
      return '\"' + name + '\"';
      //todo: print language and datatype
   }
   if( name.empty() ) return ts[node.ns_id()];
   return ts[node.ns_id()] + '#' + name;
}

/*
*******************************************************************************/
std::string to_string_short(const Node_id nid, Triple_store const& ts) {
   if( is_empty(nid) ) return "";
   Node const& node = ts[nid];
   const std::string name = node.value_str();
   const Ns_id nsid = node.ns_id();
   if( is_blank(nsid) ) return "_:" + name;
   if( is_empty(nsid) ) return '\"' + name + '\"';
   if( name.empty() ) return ts[node.ns_id()];
   const std::string pref = ts.iris().prefix(nsid);
   return pref.empty() ? ts[nsid] + '#' + name : pref + ':' + name;
}

/*
*******************************************************************************/
std::string to_string_shortest(const Node_id nid, Triple_store const& ts) {
   if( is_empty(nid) ) return "";
   Node const& node = ts[nid];
   const std::string name = node.value_str();
   const Ns_id nsid = node.ns_id();
   if( is_blank(nsid) ) return "_:" + name;
   if( is_empty(nsid) ) return '\"' + name + '\"';
   const std::string pref = ts.iris().prefix(nsid);
   return (pref.empty() ? "ns" + boost::lexical_cast<std::string>(nsid()) : pref)
            + ( name.empty() ? "" : ':' + name )
                     ;
}

}//namespace owlcpp
