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
Node_id Triple_store::insert_reference(std::string const&) {

}

/*
*******************************************************************************/
Node_id Triple_store::insert_literal(std::string const&) {

}

/*
*******************************************************************************/
Node_id Triple_store::insert_blank(std::string const&) {

}

/*
*******************************************************************************/
Doc_id Triple_store::insert_doc(
         std::string const& path,
         std::string const& iri,
         std::string const& version) {

}

/*
*******************************************************************************/
void Triple_store::insert_triple(
         const Node_id subj,
         const Node_id pred,
         const Node_id obj,
         const Doc_id doc) {

}


}//namespace owlcpp
