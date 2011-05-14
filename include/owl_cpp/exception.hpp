/** @file "/owl_cpp/include/owl_cpp/exception.hpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include "boost/exception/all.hpp"
namespace b = boost;

namespace owl_cpp{

/**
Example:
   BOOST_THROW_EXCEPTION(
         base_exception()
         << base_exception::msg_t("exception occurred")
         << base_exception::nested_t(b::current_exception())
   );
*******************************************************************************/
struct base_exception : virtual public std::exception, virtual public b::exception {
   typedef b::error_info<struct errinfo_message_, std::string> msg_t;
   typedef b::error_info<struct errinfo_str1_, std::string> str1_t;
   typedef b::error_info<struct errinfo_str2_, std::string> str2_t;
   typedef b::error_info<struct errinfo_str3_, std::string> str3_t;
   typedef b::errinfo_nested_exception nested_t;
};

}//namespace owl_cpp

#endif /* EXCEPTION_HPP_ */
