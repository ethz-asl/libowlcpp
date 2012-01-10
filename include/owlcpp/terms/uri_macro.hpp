/** @file "/owlcpp/include/owlcpp/terms/uri_macro.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef URI_MACRO_HPP_
#define URI_MACRO_HPP_
#include "owlcpp/config.hpp"
#include "owlcpp/terms/detail/uri_typename_macro.hpp"

#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/facilities/empty.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"

/** @brief Generate standard URI tag struct @details e.g., @code
 struct Ns_rdfs {
   static const unsigned index = 3;
   static const std::string name;
   static const std::string prefix;
   typedef Ns_id id_type;
}; @endcode
*******************************************************************************/
#define OWLCPP_STD_NAMESPACE_TYPE(r, d, i, e) \
struct OWLCPP_NAMESPACE_TYPE_NAME(e) { \
   static const unsigned index = BOOST_PP_ADD(d,i); \
   static std::string const & iri() { \
      static const std::string str=BOOST_PP_SEQ_ELEM(2,e); \
      return str; \
   } \
   static std::string const & prefix() { \
      static const std::string str=BOOST_PP_SEQ_ELEM(1,e); \
      return str; \
   } \
   typedef ::owlcpp::Ns_id id_type; \
   static id_type id() {return id_type(index);} \
}; \
BOOST_PP_EMPTY() \
/* */

/**@brief Generate OWL URI tag classes
*******************************************************************************/
/**@brief Generate class definitions for standard namespace IRI tags.
The classes are defined based on seq, sequence of IRI definitions.
Each definition is a sequence of 3 elements,
((class name)("standard prefix")("IRI")).
Node IDs are assigned sequentially starting from n0.
*******************************************************************************/
#define OWLCPP_GENERATE_IRI_TAGS(seq, n0) \
   BOOST_PP_SEQ_FOR_EACH_I(OWLCPP_STD_NAMESPACE_TYPE, n0, seq) \

#endif /* URI_MACRO_HPP_ */
