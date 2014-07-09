/** @file "/owlcpp/include/owlcpp/terms/term_methods.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TERM_METHODS_HPP_
#define TERM_METHODS_HPP_
#include <string>
#include <cstring>

namespace owlcpp{ namespace terms{

/** @return complete IRI for standard terms
*******************************************************************************/
template<class T> inline std::string to_string_full(const T&) {
   typedef typename T::ns_type ns_t;
   return ns_t::iri() + "#" + T::fragment();
}

/** @return prefix IRI for standard terms
*******************************************************************************/
template<class T> inline std::string to_string_pref(const T&) {
   typedef typename T::ns_type ns_t;
   return ns_t::prefix() + ":" + T::fragment();
}

/** compare string with standard terms
*******************************************************************************/
template<class T> inline bool
comparison(char const* str, const std::size_t len, T const&) {
   typedef typename T::ns_type ns_t;
   if( len != ns_t::iri().size() + T::fragment().size() + 1 ) return false;
   if(
            ns_t::iri().compare(
                     0,
                     ns_t::iri().size(),
                     str,
                     0,
                     ns_t::iri().size()
            ) != 0
   ) return false;
   if( str[ns_t::iri().size()] != '#' ) return false;
   return T::fragment().compare(
            0,
            T::fragment().size(),
            str,
            ns_t::iri().size() + 1,
            T::fragment().size()
   ) == 0;
}

/** compare string with standard terms
*******************************************************************************/
template<class T> inline bool comparison(std::string const& str, T const&) {
   typedef typename T::ns_type ns_t;
   if( str.size() != ns_t::iri().size() + T::fragment().size() + 1 ) return false;
   if(
            ns_t::iri().compare(
                     0,
                     ns_t::iri().size(),
                     str,
                     0,
                     ns_t::iri().size()
            ) != 0
   ) return false;
   if( str[ns_t::iri().size()] != '#' ) return false;
   return T::fragment().compare(
            0,
            T::fragment().size(),
            str,
            ns_t::iri().size() + 1,
            T::fragment().size()
   ) == 0;
}

/** compare string with standard terms
*******************************************************************************/
template<class T> inline bool comparison(char const* str, T const&) {
   return comparison(str, std::strlen(str), T());
}

}//namespace terms
}//namespace owlcpp

#endif /* TERM_METHODS_HPP_ */
