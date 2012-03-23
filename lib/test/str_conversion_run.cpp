/** @file "/owlcpp/lib/test/str_conversion_run.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE str_conversion_run
#include "boost/test/unit_test.hpp"
#include "str_conversion.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{ namespace test{

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( str_conversion_run_case01 ) {
   unsigned n = 0;
   BOOST_CHECK_NO_THROW( n = get_unsigned(" 42"));
   BOOST_CHECK_EQUAL(n, 42U);
   BOOST_CHECK_THROW( n = get_unsigned(" -42"), base_exception);
   BOOST_CHECK_THROW( n = get_unsigned(" 42.5"), base_exception);
   bool m = false;
   BOOST_CHECK_NO_THROW( m = get_bool(" 1"));
   BOOST_CHECK(m);
   BOOST_CHECK_THROW( m = get_bool(" 2"), base_exception);
   BOOST_CHECK_NO_THROW( m = get_bool("false  "));
   BOOST_CHECK(!m);
   BOOST_CHECK_NO_THROW( m = get_bool("    true  "));
   BOOST_CHECK(m);
}

}//namespace test
}//namespace owlcpp
