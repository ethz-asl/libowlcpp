/** @file "/owlcpp/include/owlcpp/terms/custom_type_macro.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef CUSTOM_TYPE_MACRO_HPP_
#define CUSTOM_TYPE_MACRO_HPP_
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/reverse.hpp"
#include "boost/preprocessor/stringize.hpp"

#include "owlcpp/terms/detail/term_typename_macro.hpp"
#include "owlcpp/terms/detail/uri_typename_macro.hpp"

/** @brief Generate one URI tag struct @details e.g., @code
 struct Ns_rdfs {
   static const std::string name;
   static const std::string prefix;
}; @endcode
*******************************************************************************/
#define OWLCPP_NAMESPACE_TYPE(r, d, e) struct OWLCPP_NAMESPACE_TYPE_NAME(e) { \
   static std::string const & name() { \
      static const std::string str=BOOST_PP_SEQ_ELEM(1,e); \
      return str; \
   } \
   static std::string const & prefix() { \
      static const std::string str=BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0,e)); \
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
      static const std::string str=BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(e))); \
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

#endif /* CUSTOM_TYPE_MACRO_HPP_ */
