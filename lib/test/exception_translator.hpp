/** @file "/owlcpp/lib/test/exception_translator.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXCEPTION_TRANSLATOR_HPP_
#define EXCEPTION_TRANSLATOR_HPP_
#include <string>
#include "boost/test/unit_test_monitor.hpp"
#include "owlcpp/exception.hpp"


namespace owlcpp{ namespace test{ namespace detail{

void translate(owlcpp::base_exception const& e) {
   BOOST_FAIL(boost::diagnostic_information(e));
}
}//namespace detail

/** Test fixture for printing exception info
*******************************************************************************/
struct Exception_translator {
   Exception_translator() {
      ::boost::unit_test::unit_test_monitor.
      register_exception_translator<owlcpp::base_exception>(&detail::translate);
   }
};

}//namespace test
}//namespace owlcpp
#endif /* EXCEPTION_TRANSLATOR_HPP_ */
