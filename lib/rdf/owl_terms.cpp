/** @file "/owlcpp/lib/rdf/owl_terms.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/owl_terms.hpp"

#include "boost/mpl/for_each.hpp"

#include "node_tag_vector_owl.hpp"

namespace owlcpp {

const Node_id Owl_terms::max_(OWLCPP_TERMS_NUMBER - 1);

/*
*******************************************************************************/
Owl_terms::Owl_terms()
: node_index_(OWLCPP_TERMS_NUMBER),
  empty_node_(terms::T_empty_::ns_type::id(), terms::T_empty_::name()),
  empty_id_(terms::T_empty_::id()),
  nodes_rdfs_(),
  nodes_rdf_(),
  nodes_xsd_(),
  nodes_owl_()
{
   node_index_[terms::T_empty_::id()()] = &empty_node_;
   Node_tag_inserter nti_rdfs(nodes_rdfs_, node_index_);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti_rdfs);

   Node_tag_inserter nti_rdf(nodes_rdf_, node_index_);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti_rdf);

   Node_tag_inserter nti_xsd(nodes_xsd_, node_index_);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(nti_xsd);

   Node_tag_inserter nti_owl(nodes_owl_, node_index_);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti_owl);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti_owl);
}

}//namespace owlcpp
