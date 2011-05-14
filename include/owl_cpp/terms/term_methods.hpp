/** @file "/owl_cpp/include/owl_cpp/terms/term_methods.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
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
   return ns_t::name + "#" + T::name;
}

/** @return prefix IRI for standard terms
*******************************************************************************/
template<class T> inline std::string short_name(const T&) {
   typedef typename T::ns_type ns_t;
   return ns_t::prefix + ":" + T::name;
}



}//namespace owl_cpp

#endif /* TERM_METHODS_HPP_ */
