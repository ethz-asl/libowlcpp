/** @file "/owlcpp/include/owlcpp/terms/ns_iri_tag_gen_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef NS_IRI_TAG_GEN_MACRO_HPP_
#define NS_IRI_TAG_GEN_MACRO_HPP_
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"

#include "owlcpp/terms/ns_iri_tag_macro.hpp"

#define OWLCPP_NAMESPACE_TAG_MACRO_(r, n0, i, e) \
OWLCPP_NAMESPACE_TAG_TYPE( \
         BOOST_PP_SEQ_ELEM(0,e), \
         BOOST_PP_SEQ_ELEM(1,e), \
         BOOST_PP_SEQ_ELEM(2,e), \
         BOOST_PP_ADD(n0,i) \
); \
BOOST_PP_EMPTY() \
/* */

/**@brief Generate class definitions for namespace IRI tags.
The classes are defined based on seq, sequence of IRI definitions.
Each definition is a sequence of 3 elements,
((class name)("namespace prefix")("namespace IRI")).
Namespace IDs are assigned sequentially starting from n0.
*******************************************************************************/
#define OWLCPP_GENERATE_NAMESPACE_TAGS(seq, n0) \
         BOOST_PP_SEQ_FOR_EACH_I(OWLCPP_NAMESPACE_TAG_MACRO_, n0, seq)


#endif /* NS_IRI_TAG_GEN_MACRO_HPP_ */
