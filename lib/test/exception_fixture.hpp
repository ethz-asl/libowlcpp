/** @file "/owlcpp/lib/test/exception_fixture.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXCEPTION_FIXTURE_HPP_
#define EXCEPTION_FIXTURE_HPP_
#include "boost/test/unit_test_monitor.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{ namespace test{ namespace detail{

void translate(boost::exception const& e) {
   std::cerr << boost::diagnostic_information(e) << std::endl;
   throw boost::enable_current_exception(e);
}
}//namespace detail

/** Test fixture for printing exception info
*******************************************************************************/
struct Exception_fixture {
   Exception_fixture() {
      ::boost::unit_test::unit_test_monitor.
      register_exception_translator<boost::exception>(&detail::translate);
   }
};

}//namespace test
}//namespace owlcpp
#endif /* EXCEPTION_FIXTURE_HPP_ */
