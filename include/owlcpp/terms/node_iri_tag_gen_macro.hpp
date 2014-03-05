/** @file "/owlcpp/include/owlcpp/terms/node_iri_tag_gen_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef NODE_IRI_TAG_GEN_MACRO_HPP_
#define NODE_IRI_TAG_GEN_MACRO_HPP_
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/reverse.hpp"

#include "owlcpp/terms/node_iri_tag_macro.hpp"
#include "owlcpp/terms/node_iri_tag_name_macro.hpp"

#define OWLCPP_NODE_TAG_TYPE_MACRO_(r, n0, i, e) \
OWLCPP_NODE_TAG_TYPE( \
         OWLCPP_NODE_TYPE_NAME(e), \
         BOOST_PP_SEQ_ELEM(0,e), \
         BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(e))), \
         BOOST_PP_ADD(n0,i) \
); \
BOOST_PP_EMPTY() \
/* */


/**@brief Generate class definitions for IRI node tags.
The classes are defined based on seq, sequence of node definitions.
Each definition is a sequence of 2 or 3 elements,
of which the first one refers to a standard IRI prefix.
The name of the type is formed by concatenating first and second elements.
The last element---either second or third---is used as string value of the node.
Node IDs are assigned sequentially starting from n0.
*******************************************************************************/
#define OWLCPP_GENERATE_NODE_TAGS(seq, n0) \
         BOOST_PP_SEQ_FOR_EACH_I(OWLCPP_NODE_TAG_TYPE_MACRO_, n0, seq)

#endif /* NODE_IRI_TAG_GEN_MACRO_HPP_ */
