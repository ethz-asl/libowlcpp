/** @file "/owl_cpp/lib/type_vector.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef TYPE_VECTOR_HPP_
#define TYPE_VECTOR_HPP_
#include "owl_cpp/terms/uri_tags.hpp"
#include "owl_cpp/terms/term_tags.hpp"

// include appropriate boost::mpl vector headers
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_NAMESPACES_ALL)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_RDFS)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_RDF)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_OWL1)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_OWL2)
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_VISITABLE)

namespace owl_cpp{ namespace terms{

/**
@brief typelist of common OWL namespace tags
*******************************************************************************/
typedef OWLCPP_NAMESPACE_MPL_VECTOR(OWLCPP_NAMESPACES_ALL)
      mpl_vector_namespaces_all_t;

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
@brief typelist of other term tags
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_MISC) mpl_vector_terms_other_t;

/**
@brief typelist of OWL term tags to generate visitable triple classes
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_VISITABLE)
      mpl_vector_terms_visitable_t;

}//namespace keywords
}//namespace owl_cpp
#endif /* TYPE_VECTOR_HPP_ */