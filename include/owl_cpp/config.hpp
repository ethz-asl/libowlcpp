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

#endif /* CONFIG_HPP_ */
