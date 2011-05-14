/** @file "/owl_cpp/lib/str_conversion.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef STR_CONVERSION_HPP_
#define STR_CONVERSION_HPP_
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
namespace b = boost;
#include "boost/algorithm/string/trim.hpp"
namespace ba = boost::algorithm;
#include "boost/throw_exception.hpp"
#include "owl_cpp/exception.hpp"

namespace owl_cpp{

/**
*******************************************************************************/
inline unsigned get_unsigned(const std::string& str) {
   try{
      return b::numeric_cast<unsigned>(b::lexical_cast<int>( ba::trim_copy(str) ));
   } catch(...) {
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("not an unsigned integer")
            << base_exception::str1_t("\"" + str + "\"")
      );
   }
}

/**
An instance of a datatype that is defined as ·boolean·
can have the following legal literals {true, false, 1, 0}.
*******************************************************************************/
inline bool get_bool(const std::string& str) {
   const std::string str2 = ba::trim_copy(str);
   if( str2 == "1" || str2 == "true" ) return true;
   if( str2 == "0" || str2 == "false" ) return false;
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("not a boolean")
            << base_exception::str1_t("\"" + str + "\"")
      );
}

}//namespace owl_cpp

#endif /* STR_CONVERSION_HPP_ */
