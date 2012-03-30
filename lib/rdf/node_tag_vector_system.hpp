/** @file "/owlcpp/lib/rdf/node_tag_vector_system.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TAG_VECTOR_SYSTEM_HPP_
#define NODE_TAG_VECTOR_SYSTEM_HPP_


#include "owlcpp/terms/node_tags_system.hpp"
#include "type_vector_macro.hpp"
// include appropriate boost::mpl vector header
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_NODE_LIST_SYSTEM)

namespace owlcpp{ namespace terms{

/**@brief typelist of system node tags
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_NODE_LIST_SYSTEM) mpl_vector_terms_system_t;

}//namespace keywords
}//namespace owlcpp
#endif /* NODE_TAG_VECTOR_SYSTEM_HPP_ */
