/** @file "/owlcpp/lib/rdf/triple_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp {

/*
*******************************************************************************/
Node_id Triple_store::insert_reference(std::string const& iri) {
   const std::size_t n = iri.find('#');
   if( std::string::npos == n ) {
      const Ns_id nid = iri_.insert(iri);
      return node_.insert(Node(nid, ""));
   }
   const Ns_id nid = iri_.insert(iri.substr(0,n));
   return node_.insert( Node(nid, iri.substr(n+1)) );
}

/*
*******************************************************************************/
Node_id Triple_store::insert_literal(std::string const&) {

   //todo:
}

/*
*******************************************************************************/
Node_id Triple_store::insert_blank(std::string const&) {

   //todo:
}

/*
*******************************************************************************/
Doc_id Triple_store::insert_doc(
         std::string const& path,
         std::string const& iri,
         std::string const& version) {

   //todo:
}

/*
*******************************************************************************/
void Triple_store::insert_triple(
         const Node_id subj,
         const Node_id pred,
         const Node_id obj,
         const Doc_id doc) {

   //todo:
}


/*
*******************************************************************************/
Doc_id const* Triple_store::find_doc(std::string const& iri) const {
   //todo:
/*
   version_index_t const& v_ind = store_.get<version_tag>();
   version_iter_t v_iter = v_ind.find(id);
   if( v_iter != v_ind.end() ) return &v_iter->id_;

   iri_index_t const& iri_ind = store_.get<iri_tag>();
   iri_iter_t iri_iter = iri_ind.find(id);
   if( iri_iter != iri_ind.end() ) return &iri_iter->id_;

   return 0;
*/
}


}//namespace owlcpp
