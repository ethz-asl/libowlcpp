/** @file "/owl_cpp/include/owl_cpp/terms/term_methods.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TERM_METHODS_HPP_
#define TERM_METHODS_HPP_
#include <string>
//#include "owl_cpp/ns_id.hpp"

namespace owl_cpp{

/** @return complete IRI for standard terms
*******************************************************************************/
template<class T> inline std::string full_name(const T&) {
   typedef typename T::ns_type ns_t;
   return ns_t::iri() + "#" + T::name();
}

/** @return prefix IRI for standard terms
*******************************************************************************/
template<class T> inline std::string short_name(const T&) {
   typedef typename T::ns_type ns_t;
   return ns_t::prefix() + ":" + T::name();
}



}//namespace owl_cpp

#endif /* TERM_METHODS_HPP_ */
