/** @file "/owlcpp/lib/rdf/test/map_triple_3_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE map_triple_3_run
#include "boost/test/unit_test.hpp"
#include "boost/mpl/assert.hpp"

#include "test/exception_fixture.hpp"
#include "test/test_utils.hpp"
#include "owlcpp/rdf/map_triple.hpp"

namespace owlcpp{ namespace test{

/**@test inserting triples
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_triple<> mt;
   BOOST_CHECK_EQUAL(mt.size(), 0U);

   mt.insert(triple(0,0,0,0));
   BOOST_CHECK_EQUAL(mt.size(), 1U);

   insert_seq(mt, random_triples1);
   BOOST_CHECK_EQUAL(mt.size(), 21U);
}

/**@test search query type
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Map_triple<> mt;

}

}//namespace test
}//namespace owlcpp
