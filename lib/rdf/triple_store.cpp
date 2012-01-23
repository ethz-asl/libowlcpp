/** @file "/owlcpp/lib/rdf/triple_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/iri_tags_system.hpp"

namespace owlcpp {

/*
Node_id Triple_store::insert_iri_node(std::string const& iri) {
   const std::size_t n = iri.find('#');
   if( std::string::npos == n ) {
      const Ns_id nid = iri_.insert(iri);
      return node_.insert(Node(nid, ""));
   }
   const Ns_id nid = iri_.insert(iri.substr(0,n));
   return node_.insert( Node(nid, iri.substr(n+1)) );
}
*******************************************************************************/

/*
Node_id Triple_store::insert_lit_node(std::string const& val) {
   return node_.insert( Node(terms::N_empty::id(), val) );
}
*******************************************************************************/

/*
Node_id Triple_store::insert_blank_node(std::string const& name) {
   return node_.insert( Node(terms::N_blank::id(), name) );
}
*******************************************************************************/

/*
Doc_id Triple_store::insert_doc(
         std::string const& path,
         std::string const& iri,
         std::string const& version
) {
   const Node_id iid = insert_iri_node(iri);
   if( version.empty() ) return doc_.insert(path, iid);
   const Node_id vid = insert_iri_node(version);
   return doc_.insert(path, iid, vid);
}
*******************************************************************************/

/*
Node_id const* Triple_store::find_iri_node(std::string const& iri) const {
   const std::size_t n = iri.find('#');
   Ns_id const * iid;
   if( std::string::npos == n ) {
      iid = iri_.find_iri(iri);
      if( ! iid ) return 0;
      return node_.find( Node(*iid, ""));
   } else {
      iid = iri_.find_iri(iri.substr(0, n));
      if( ! iid ) return 0;
      return node_.find( Node(*iid, iri.substr(n+1)));
   }
}
*******************************************************************************/

/*
Doc_id const* Triple_store::find_doc(std::string const& iri) const {
   Node_id const* nid = find_iri_node(iri);
   if( ! nid ) return 0;
   const Doc_map::version_range vr = doc_.find_version(*nid);
   if( vr ) return &vr.front();
   const Doc_map::iri_range ir = doc_.find_iri(*nid);
   if( ir ) return &ir.front();
   return 0;
}
*******************************************************************************/

/*
Node const* Triple_store::version(const Doc_id did) const {
   Node_id const* nid = doc_.version(did);
   if( nid ) return &node_[*nid];
   return 0;
}
*******************************************************************************/

/*
*******************************************************************************/
}//namespace owlcpp
