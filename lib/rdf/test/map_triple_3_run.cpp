/** @file "/owlcpp/lib/rdf/test/map_triple_3_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE map_triple_3_run
#include "boost/test/unit_test.hpp"

#include "test/exception_fixture.hpp"
#include "test/test_utils.hpp"
#include "owlcpp/rdf/map_triple2.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_triple<> m;
   BOOST_CHECK_EQUAL(m.size(), 0U);

   m.insert(triple(0,0,0,0));
   BOOST_CHECK_EQUAL(m.size(), 1U);
}

}//namespace test
}//namespace owlcpp
