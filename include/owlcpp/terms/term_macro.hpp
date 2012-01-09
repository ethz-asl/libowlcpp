/** @file "/owlcpp/include/owlcpp/terms/term_macro.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TERM_MACRO_HPP_
#define TERM_MACRO_HPP_
#include "owlcpp/terms/detail/term_typename_macro.hpp"
#include "owlcpp/terms/term_list.hpp"
#include "owlcpp/config.hpp"

#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/enum.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/reverse.hpp"

#include "owlcpp/node_id.hpp"

/**@brief Generate light-weight type definition for standard OWL term
@details OWLCPP_STD_TERM_TYPE( , , 2, (rdfs)(subClassOf))
@code
struct T_owl_allValuesFrom {
   typedef N_owl ns_type;
   typedef ::owlcpp::Node_id id_type;
   static const unsigned index = 4;
   static std::string const & name() {
      static const std::string str("allValuesFrom");
      return str;
   }
   static ::owlcpp::Node_id id() {return ::owlcpp::Node_id(index);}
}; @endcode
*******************************************************************************/
#define OWLCPP_STD_TERM_TYPE(r, d, i, e) \
   struct OWLCPP_TERM_TYPE_NAME(e) { \
   typedef OWLCPP_NAMESPACE_TYPE_NAME(e) ns_type; \
   typedef ::owlcpp::Node_id id_type; \
   static const unsigned index = i; \
   static std::string const & name() { \
      static const std::string str = std::string(BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(e)))); \
      return str; \
   } \
   static id_type id() {return id_type(index);} \
}; \
BOOST_PP_EMPTY() \
/* */

/**@brief Generate class definitions for supported standard OWL term tags
*******************************************************************************/
#define OWLCPP_GENERATE_STD_TERM_TYPES \
   BOOST_PP_SEQ_FOR_EACH_I( \
         OWLCPP_STD_TERM_TYPE, \
         , \
         OWLCPP_TERMS_ALL \
   ) \
/* */

/**@brief Generate name member initializations for the supported triple types
@details OWLCPP_INITIALIZE_TERM_NAME_STRINGS
*******************************************************************************/
#define OWLCPP_INIT_STD_TERM_NAME_STRINGS \
   BOOST_PP_SEQ_FOR_EACH(OWLCPP_TERM_INIT, , OWLCPP_TERMS_ALL)

#endif /* TERM_MACRO_HPP_ */
