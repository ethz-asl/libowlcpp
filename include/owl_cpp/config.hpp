/** @file "/owl_cpp/include/owl_cpp/config.hpp" 
part of owl_cpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2011 @author Mikhail K Levin
*******************************************************************************/
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "boost/config.hpp"
#ifndef BOOST_SYMBOL_EXPORT
  #define BOOST_SYMBOL_EXPORT
  #define BOOST_SYMBOL_IMPORT
#endif
#if defined(OWLCPP_DYN_LINK)
#  if defined(OWLCPP_SOURCE)
#     define OWLCPP_DECL BOOST_SYMBOL_EXPORT
#  else
#     define OWLCPP_DECL BOOST_SYMBOL_IMPORT
#  endif
#else
#  define OWLCPP_DECL
#endif

#ifdef BOOST_MSVC
   #pragma warning (push)
   #pragma warning (disable : 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
   #pragma warning (disable : 4290) // C++ exception specification ignored except to ...
   #pragma warning (disable : 4355) // 'this' : used in base member initializer list
#endif

#endif /* CONFIG_HPP_ */
