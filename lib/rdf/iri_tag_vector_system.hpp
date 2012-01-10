/** @file "/owlcpp/lib/rdf/iri_tag_vector_system.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef IRI_TAG_VECTOR_SYSTEM_HPP_
#define IRI_TAG_VECTOR_SYSTEM_HPP_

#include "owlcpp/terms/iri_tags_system.hpp"
#include "type_vector_macro.hpp"
// include appropriate boost::mpl vector header
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_IRI_LIST_SYSTEM)

namespace owlcpp{ namespace terms{

/**@brief typelist of common OWL namespace tags
*******************************************************************************/
typedef OWLCPP_NAMESPACE_MPL_VECTOR(OWLCPP_IRI_LIST_SYSTEM)
      mpl_vector_iris_system_t;



}//namespace terms
}//namespace owlcpp
#endif /* IRI_TAG_VECTOR_SYSTEM_HPP_ */
