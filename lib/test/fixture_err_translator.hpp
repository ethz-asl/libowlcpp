/** @file "/owlcpp/lib/test/fixture.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef FIXTURE_HPP_
#define FIXTURE_HPP_
#include <string>
#include "boost/test/unit_test_monitor.hpp"
#include "owlcpp/exception.hpp"


namespace owlcpp{ namespace test{ namespace detail{

void translate(boost::exception const& e) {
   BOOST_FAIL(boost::diagnostic_information(e));
}
}//namespace detail

/** Test fixture for printing exception info
*******************************************************************************/
struct Fixture_err_translator {
   Fixture_err_translator() {
      ::boost::unit_test::unit_test_monitor.
      register_exception_translator<boost::exception>(&detail::translate);
   }
};

}//namespace test
}//namespace owlcpp
#endif /* FIXTURE_HPP_ */
