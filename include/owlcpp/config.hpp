/** @file "/owlcpp/include/owlcpp/config.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "boost/config.hpp"

#ifdef BOOST_MSVC
   #pragma warning (push)
   #pragma warning (disable : 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
   #pragma warning (disable : 4275) // non DLL-interface classkey "identifier" used as base for DLL-interface
   #pragma warning (disable : 4290) // C++ exception specification ignored except to ...
   #pragma warning (disable : 4355) // 'this' : used in base member initializer list
   #pragma warning (disable : 4800) // forcing value to bool 'true' or 'false'
   #pragma warning (disable : 4003) // not enough actual parameters for macro
   #pragma warning (disable : 4503) // not enough actual parameters for macro
#endif


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

#endif /* CONFIG_HPP_ */
