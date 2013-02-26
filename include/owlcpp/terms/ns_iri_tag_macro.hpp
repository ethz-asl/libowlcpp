/** @file "/owlcpp/include/owlcpp/terms/ns_iri_tag_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef NS_IRI_TAG_MACRO_HPP_
#define NS_IRI_TAG_MACRO_HPP_
#include <string>
#include "owlcpp/ns_id.hpp"

/** @brief Generate namespace IRI tag struct
@details e.g.,
@code
OWLCPP_NAMESPACE_TAG_TYPE(
   rdfs, "rdfs", "http://www.w3.org/2000/01/rdf-schema", 3
); @endcode
expands to
@code
 struct rdfs {
   static const unsigned index = 3;
   static std::string const & iri() {
      static const std::string str="http://www.w3.org/2000/01/rdf-schema";
      return str;
   }
   static std::string const & prefix() {
      static const std::string str="rdfs";
      return str;
   }
   typedef ::owlcpp::Ns_id id_type;
   static id_type id() {return id_type(index);}
}; @endcode
*******************************************************************************/
#define OWLCPP_NAMESPACE_TAG_TYPE(name, prefix_str, iri_str, i) struct name { \
   typedef ::owlcpp::Ns_id id_type; \
   static const id_type::value_type index = i; \
   static std::string const & iri() { \
      static const std::string str=iri_str; \
      return str; \
   } \
   static std::string const & prefix() { \
      static const std::string str=prefix_str; \
      return str; \
   } \
   static id_type id() {return id_type(index);} \
}
/* */

#endif /* NS_IRI_TAG_MACRO_HPP_ */
