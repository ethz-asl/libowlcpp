/** @file "/owlcpp/lib/rdf/node_tag_vector_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TAG_VECTOR_OWL_HPP_
#define NODE_TAG_VECTOR_OWL_HPP_

#include "owlcpp/terms/node_tags_owl.hpp"
#include "type_vector_macro.hpp"
// include appropriate boost::mpl vector headers
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_RDFS)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_RDF)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_OWL1)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_OWL2)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_XSD)

namespace owlcpp{ namespace terms{

/**
@brief typelist of OWL term tags from RDFS namespace
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_RDFS) mpl_vector_terms_rdfs_t;

/**
@brief typelist of OWL term tags from RDF namespace
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_RDF) mpl_vector_terms_rdf_t;

/**
@brief typelist of OWL term tags from OWL namespace
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_OWL1) mpl_vector_terms_owl1_t;
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_OWL2) mpl_vector_terms_owl2_t;

/**
@brief typelist of OWL term tags from XSD namespace
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_XSD) mpl_vector_terms_xsd_t;

}//namespace keywords
}//namespace owlcpp
#endif /* NODE_TAG_VECTOR_OWL_HPP_ */
