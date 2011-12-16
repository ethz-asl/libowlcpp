/** @file "/owl_cpp/include/owl_cpp/terms/custom_type_macro.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef CUSTOM_TYPE_MACRO_HPP_
#define CUSTOM_TYPE_MACRO_HPP_
#include "boost/preprocessor/arithmetic/inc.hpp"
#include "boost/preprocessor/arithmetic/dec.hpp"
#include "boost/preprocessor/arithmetic/div.hpp"
#include "boost/preprocessor/arithmetic/mul.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/enum.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/reverse.hpp"
#include "boost/preprocessor/seq/size.hpp"
#include "boost/preprocessor/stringize.hpp"

/**@brief First symbols of namespace type names
*******************************************************************************/
#define OWLCPP_NS_TYPE_PREFIX N_

/**@brief Generate namespace type name
*******************************************************************************/
#define OWLCPP_NAMESPACE_TYPE_NAME(p) \
   BOOST_PP_CAT(OWLCPP_NS_TYPE_PREFIX, BOOST_PP_SEQ_ELEM(0,p))

/** @brief Generate one URI tag struct @details e.g., @code
 struct Ns_rdfs {
   static const std::string name;
   static const std::string prefix;
}; @endcode
*******************************************************************************/
#define OWLCPP_NAMESPACE_TYPE(r, d, e) struct OWLCPP_NAMESPACE_TYPE_NAME(e) { \
   static std::string const & name() { \
      static const std::string str(BOOST_PP_SEQ_ELEM(1,e)); \
      return str; \
   } \
   static std::string const & prefix() { \
      static const std::string str(BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0,e))); \
      return str; \
   } \
}; \
BOOST_PP_EMPTY() \
/* */

/**@brief Generate custom namespace URI tag classes
*******************************************************************************/
#define OWLCPP_GENERATE_NAMESPACE_TYPES(seq) \
   BOOST_PP_SEQ_FOR_EACH(OWLCPP_NAMESPACE_TYPE, , seq) \
/* */


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

#define OWLCPP_NS_TYPESEQ_MACRO(r, d, e) (OWLCPP_NAMESPACE_TYPE_NAME(e))

/**@brief Generate comma-separated list of standard URI types
*******************************************************************************/
#define OWLCPP_NAMESPACE_ENUM(seq) \
      BOOST_PP_SEQ_ENUM( BOOST_PP_SEQ_FOR_EACH( OWLCPP_NS_TYPESEQ_MACRO, , seq ) ) \
/* */

#define OWLCPP_MPL_VECTOR(seq) BOOST_PP_CAT(::boost::mpl::vector, BOOST_PP_SEQ_SIZE(seq))

/** e.g., @code
boost::mpl::vector48<
   T_rdfs_subClassOf, T_rdfs_subPropertyOf, ...   > @endcode
*******************************************************************************/
#define OWLCPP_NAMESPACE_MPL_VECTOR(seq) \
   OWLCPP_MPL_VECTOR(seq)<OWLCPP_NAMESPACE_ENUM(seq)>


/**@brief Standard OWL predicate type names begin from this */
#define OWLCPP_TERM_TYPE_PREFIX T_

/**@brief Generate type name @details e.g.,
OWLCPP_TERM_TYPE_NAME((rdfs)(subClassOf)) produces T_rdfs_subClassOf
*******************************************************************************/
#define OWLCPP_TERM_TYPE_NAME(n) \
      BOOST_PP_CAT( \
            OWLCPP_TERM_TYPE_PREFIX, BOOST_PP_CAT( \
                  BOOST_PP_SEQ_ELEM(0,n), BOOST_PP_CAT( \
                        _, BOOST_PP_SEQ_ELEM(1,n) \
                  ) \
            ) \
      ) \
/* */


/**@brief Generate light-weight type definition for standard OWL term
@details OWLCPP_STD_TERM_TYPE( , , 2, (rdfs)(subClassOf))
@code
struct T_ro_has_participant {
   typedef N_ro ns_type;
   static std::string const & name() {
      static const std::string str("has_participant");
      return str;
   }
};
@endcode
*******************************************************************************/
#define OWLCPP_TERM_TYPE(r, d, e) \
   struct OWLCPP_TERM_TYPE_NAME(e) { \
   typedef OWLCPP_NAMESPACE_TYPE_NAME(e) ns_type; \
   static std::string const & name() { \
      static const std::string str(BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(e)))); \
      return str; \
   } \
}; \
BOOST_PP_EMPTY() \
/* */

/**@brief Generate class definitions for term tags
*******************************************************************************/
#define OWLCPP_GENERATE_TERM_TYPES(seq) \
   BOOST_PP_SEQ_FOR_EACH(OWLCPP_TERM_TYPE, , seq) \
/* */

#define OWLCPP_TERM_ENUM_MACRO(r, d, e) (OWLCPP_TERM_TYPE_NAME(e))

/**@brief Generate comma-separated list of standard triplet types
@details OWLCPP_STD_TERM_ENUM(OWLCPP_STD_TERM_SEQ)
@code T_rdfs_subClassOf, T_rdfs_subPropertyOf, T_rdfs_domain @endcode
*******************************************************************************/
#define OWLCPP_TERM_ENUM(seq) \
      BOOST_PP_SEQ_ENUM( BOOST_PP_SEQ_FOR_EACH(OWLCPP_TERM_ENUM_MACRO, , seq) )

/**@brief
@details e.g., @code
boost::mpl::vector48<
   T_rdfs_subClassOf, T_rdfs_subPropertyOf, ...   > @endcode
*******************************************************************************/
#define OWLCPP_TERM_MPL_VECTOR(seq) OWLCPP_MPL_VECTOR(seq)<OWLCPP_TERM_ENUM(seq)>

#endif /* CUSTOM_TYPE_MACRO_HPP_ */
