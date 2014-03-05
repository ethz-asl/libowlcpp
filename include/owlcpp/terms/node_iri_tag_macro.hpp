/** @file "/owlcpp/include/owlcpp/terms/node_iri_tag_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef NODE_IRI_TAG_MACRO_HPP_
#define NODE_IRI_TAG_MACRO_HPP_
#include <string>
#include "owlcpp/node_id.hpp"

/**@brief Generate light-weight type definition for standard OWL term
@details
@code OWLCPP_NODE_TAG_TYPE(owl_allValuesFrom, owl, "allValuesFrom", 4); @endcode
@code
struct owl_allValuesFrom {
   typedef owl ns_type;
   typedef ::owlcpp::Node_id id_type;
   static const unsigned index = 4;
   static std::string const & name() {
      static const std::string str = std::string("allValuesFrom");
      return str;
   }
   static ns_type id() {return ::owlcpp::Node_id(index);}
}; @endcode
*******************************************************************************/
#define OWLCPP_NODE_TAG_TYPE(name, ns, frag_id, i) struct name { \
         typedef ns ns_type; \
         typedef ::owlcpp::Node_id id_type; \
         static const id_type::value_type index = i; \
         static std::string const& fragment() { \
            static const std::string str = std::string(frag_id); \
            return str; \
         } \
         static id_type id() {return id_type(index);} \
}
/* */

#endif /* NODE_IRI_TAG_MACRO_HPP_ */
