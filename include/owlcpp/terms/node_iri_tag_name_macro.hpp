/** @file "/owlcpp/include/owlcpp/terms/node_iri_tag_name_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012-3
*******************************************************************************/
#ifndef NODE_IRI_TAG_NAME_MACRO_HPP_
#define NODE_IRI_TAG_NAME_MACRO_HPP_

#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/cat.hpp"

/**@brief Generate type name @details e.g.,
OWLCPP_TERM_TYPE_NAME((rdfs)(subClassOf)) produces rdfs_subClassOf
*******************************************************************************/
#define OWLCPP_NODE_TYPE_NAME(n) \
         BOOST_PP_CAT( \
                  BOOST_PP_SEQ_ELEM(0,n), \
                  BOOST_PP_CAT(_, BOOST_PP_SEQ_ELEM(1,n)) \
         ) \
/* */

#endif /* NODE_IRI_TAG_NAME_MACRO_HPP_ */
