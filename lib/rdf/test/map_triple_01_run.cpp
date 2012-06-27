/** @file "/owlcpp/lib/rdf/test/map_triple_01_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_01_run
#include "boost/test/unit_test.hpp"
#include "boost/range.hpp"
#include "test/exception_fixture.hpp"
#include <iostream>

#include "owlcpp/rdf/map_triple.hpp"
#include "test_utils.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const unsigned t[][4] = {
         {0,1,0,0},
         {0,2,0,0},
         {0,3,0,0},
         {1,3,0,0},
         {0,3,1,0},
         {0,3,0,1},
         {0,4,0,0},
};

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef Map_triple<1,0,0,0> triple_map;
   triple_map tm;
   BOOST_CHECK_EQUAL(tm.size(), 0U);
   insert_seq(tm, t);
   BOOST_CHECK_EQUAL(tm.size(), 7U);
   triple_map::result_b<1,1,0,0>::type r1 = tm.find(Node_id(2), Node_id(3), any(), any());
   BOOST_CHECK(!r1);
   triple_map::result_b<1,1,0,0>::type r2 = tm.find(Node_id(0), Node_id(1), any(), any());
   BOOST_CHECK(r2);
   BOOST_CHECK_EQUAL(boost::distance(r2), 1);
   triple_map::result_b<0,1,0,0>::type r3 = tm.find(any(), Node_id(3), any(), any());
   BOOST_CHECK_EQUAL(boost::distance(r3), 4);

   BOOST_CHECK_THROW( tm.erase(triple(0,13,0,1)), Rdf_err );
   tm.erase(triple(0,3,0,1));
   BOOST_CHECK_EQUAL(tm.size(), 6U);

   tm.clear();
   BOOST_CHECK_EQUAL(tm.size(), 0U);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef Map_triple<0,1,0,0> triple_map;
   triple_map tm;
   insert_seq(tm, t);
   typedef triple_map::result_b<0,1,0,0> search;
   search::type r = tm.find(any(), Node_id(3), any(), any());
}

}//namespace test
}//namespace owlcpp
