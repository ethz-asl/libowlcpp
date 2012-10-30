/** @file "/owlcpp/include/owlcpp/exception.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include "boost/exception/all.hpp"

namespace owlcpp{
class Node_id;

/**
Example:
   BOOST_THROW_EXCEPTION(
         base_exception()
         << base_exception::msg_t("exception occurred")
         << base_exception::nested_t(boost::current_exception())
   );
*******************************************************************************/
struct base_exception : virtual public std::exception, virtual public boost::exception {
   typedef boost::error_info<struct errinfo_message_, std::string> msg_t;
   typedef boost::error_info<struct errinfo_str1_, std::string> str1_t;
   typedef boost::error_info<struct errinfo_str2_, std::string> str2_t;
   typedef boost::error_info<struct errinfo_str3_, std::string> str3_t;
   typedef boost::error_info<struct errinfo_int1_, int> int1_t;
   typedef boost::error_info<struct errinfo_int2_, int> int2_t;
   typedef boost::error_info<struct errinfo_node_id_, Node_id> node_id_t;
   typedef boost::errinfo_nested_exception nested_t;
};

}//namespace owlcpp

#endif /* EXCEPTION_HPP_ */
