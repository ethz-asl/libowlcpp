/** @file "/owlcpp/include/owlcpp/rdf/detail/map_triple_config_macro.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef MAP_TRIPLE_CONFIG_MACRO_HPP_
#define MAP_TRIPLE_CONFIG_MACRO_HPP_
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/for.hpp" //required for for_each_i.hpp
#include "boost/preprocessor/punctuation/comma_if.hpp"
#include "boost/preprocessor/seq/enum.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/size.hpp"
#include "boost/preprocessor/seq/transform.hpp"

#define OWLCPP_TRIPLE_INDEX_ELEMENT_MACRO_(s, data, elem)                     \
   ::owlcpp::BOOST_PP_CAT(elem, _tag)                                         \
/*
*/

#define OWLCPP_TRIPLE_INDEX_CONFIG_MACRO_(r, data, i, elem)                   \
   BOOST_PP_COMMA_IF(i)                                                       \
   ::boost::mpl::vector4<BOOST_PP_SEQ_ENUM(                                   \
            BOOST_PP_SEQ_TRANSFORM(                                           \
                     OWLCPP_TRIPLE_INDEX_ELEMENT_MACRO_, , elem               \
            )                                                                 \
   )>                                                                         \
/*
*/

#define OWLCPP_TRIPLE_INDEX_CONFIG(tuple_seq)                                 \
   ::boost::mpl::BOOST_PP_CAT(vector, BOOST_PP_SEQ_SIZE(tuple_seq))<          \
       BOOST_PP_SEQ_FOR_EACH_I(OWLCPP_TRIPLE_INDEX_CONFIG_MACRO_, ,tuple_seq) \
   >                                                                          \
/*
*/

#endif /* MAP_TRIPLE_CONFIG_MACRO_HPP_ */
