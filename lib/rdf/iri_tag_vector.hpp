/** @file "/owlcpp/lib/rdf/iri_tag_vector.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef IRI_TAG_VECTOR_HPP_
#define IRI_TAG_VECTOR_HPP_
#include "owlcpp/terms/ns_iri_tags.hpp"
#include "type_vector_macro.hpp"
// include appropriate boost::mpl vector header
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_NAMESPACE_IRI_LIST)

namespace owlcpp{ namespace terms{

/**@brief typelist of standard OWL namespace tags
*******************************************************************************/
typedef OWLCPP_NAMESPACE_MPL_VECTOR(OWLCPP_NAMESPACE_IRI_LIST) mpl_vector_iris_t;



}//namespace terms
}//namespace owlcpp
#endif /* IRI_TAG_VECTOR_HPP_ */
