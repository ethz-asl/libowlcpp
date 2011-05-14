/** @file "/owl_cpp/lib/test/str_conversion_run.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE str_conversion_run
#include "boost/test/unit_test.hpp"
#include "str_conversion.hpp"
#include "owl_cpp/exception.hpp"

namespace owl_cpp{ namespace test{

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
}//namespace owl_cpp
