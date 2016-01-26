/** @file "/owlcpp/lib/rdf/type_vector_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011-2
*******************************************************************************/
#ifndef TYPE_VECTOR_MACRO_HPP_
#define TYPE_VECTOR_MACRO_HPP_
#include "boost/preprocessor/arithmetic/inc.hpp"
#include "boost/preprocessor/arithmetic/dec.hpp"
#include "boost/preprocessor/arithmetic/div.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/preprocessor/seq/size.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/enum.hpp"
#include "boost/preprocessor/seq/for_each.hpp"

#include "owlcpp/terms/node_iri_tag_name_macro.hpp"

/**@brief Round up number
*******************************************************************************/
#define OWLCPP_ROUND_UP(n) \
      BOOST_PP_CAT( BOOST_PP_INC( BOOST_PP_DIV( BOOST_PP_DEC(n), 10) ), 0 ) \
/* */

/**@brief Generate name for appropriate boost/mpl/vector/vectorNN.hpp header
@details OWLCPP_STD_TERM_MPL_VECTORnn_HPP(OWLCPP_STD_TERM_SEQ) produces
"boost/mpl/vector/vector50.hpp"
*******************************************************************************/
#define OWLCPP_MPL_VECTORnn_HPP(seq) \
      BOOST_PP_STRINGIZE( \
            BOOST_PP_CAT( \
                  boost/mpl/vector/vector,OWLCPP_ROUND_UP(BOOST_PP_SEQ_SIZE(seq)).hpp \
            ) \
      ) \
/* */

#define OWLCPP_NS_TYPESEQ_MACRO(r, d, e) (BOOST_PP_SEQ_ELEM(0,e))

/**@brief Generate comma-separated list of standard URI types
*******************************************************************************/
#define OWLCPP_NAMESPACE_ENUM(seq) \
      BOOST_PP_SEQ_ENUM( BOOST_PP_SEQ_FOR_EACH( OWLCPP_NS_TYPESEQ_MACRO, , seq ) ) \
/* */

#define OWLCPP_MPL_VECTOR(seq) BOOST_PP_CAT(::boost::mpl::vector, BOOST_PP_SEQ_SIZE(seq))

/** @details e.g., @code
boost::mpl::vector48<
   rdfs_subClassOf, rdfs_subPropertyOf, ...   > @endcode
*******************************************************************************/
#define OWLCPP_NAMESPACE_MPL_VECTOR(seq) \
   OWLCPP_MPL_VECTOR(seq)<OWLCPP_NAMESPACE_ENUM(seq)>

#define OWLCPP_TERM_ENUM_MACRO(r, d, e) (OWLCPP_NODE_TYPE_NAME(e))

/**@brief Generate comma-separated list of standard triplet types
@details OWLCPP_STD_TERM_ENUM(OWLCPP_STD_TERM_SEQ)
@code rdfs_subClassOf, rdfs_subPropertyOf, rdfs_domain @endcode
*******************************************************************************/
#define OWLCPP_TERM_ENUM(seq) \
      BOOST_PP_SEQ_ENUM( BOOST_PP_SEQ_FOR_EACH(OWLCPP_TERM_ENUM_MACRO, , seq) )

/**@brief
@details e.g., @code
boost::mpl::vector48<
   rdfs_subClassOf, rdfs_subPropertyOf, ...   > @endcode
*******************************************************************************/
#define OWLCPP_TERM_MPL_VECTOR(seq) OWLCPP_MPL_VECTOR(seq)<OWLCPP_TERM_ENUM(seq)>

#endif /* TYPE_VECTOR_MACRO_HPP_ */
