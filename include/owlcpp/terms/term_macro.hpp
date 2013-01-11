/** @file "/owlcpp/include/owlcpp/terms/term_macro.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TERM_MACRO_HPP_
#define TERM_MACRO_HPP_
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/enum.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/reverse.hpp"
#include "boost/preprocessor/stringize.hpp"

#include "owlcpp/terms/detail/uri_typename_macro.hpp"
#include "owlcpp/terms/detail/term_typename_macro.hpp"

/**@brief Generate light-weight type definition for standard OWL term
@details OWLCPP_STD_TERM_TYPE( , , 2, (rdfs)(subClassOf))
@code
struct owl_allValuesFrom {
   typedef owl ns_type;
   typedef ::owlcpp::Node_id id_type;
   static const unsigned index = 4;
   static std::string const & name() {
      static const std::string str("allValuesFrom");
      return str;
   }
   static ::owlcpp::Node_id id() {return ::owlcpp::Node_id(index);}
}; @endcode
*******************************************************************************/
#define OWLCPP_NODE_TYPE(r, d, i, e) \
   struct OWLCPP_TERM_TYPE_NAME(e) { \
   typedef OWLCPP_NAMESPACE_TYPE_NAME(e) ns_type; \
   typedef ::owlcpp::Node_id id_type; \
   static const unsigned index = BOOST_PP_ADD(d,i); \
   static std::string const & name() { \
      static const std::string str = std::string(BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(e)))); \
      return str; \
   } \
   static id_type id() {return id_type(index);} \
}; \
BOOST_PP_EMPTY() \
/* */

/**@brief Generate class definitions for standard node tags.
The classes are defined based on seq, sequence of node definitions.
Each definition is a sequence of 2 or 3 elements,
of which the first one refers to a standard IRI prefix.
The name of the type is formed by concatenating first and second elements.
The last element---either second or third---is used as string value of the node.
Node IDs are assigned sequentially starting from n0.
*******************************************************************************/
#define OWLCPP_GENERATE_NODE_TAGS(seq, n0) \
   BOOST_PP_SEQ_FOR_EACH_I(OWLCPP_NODE_TYPE, n0, seq) \

#endif /* TERM_MACRO_HPP_ */
