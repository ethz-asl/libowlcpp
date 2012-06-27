/** @file "/owlcpp/lib/rdf/test/map_triple_02_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_02_run
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
   insert_seq(tm, t);
   typedef triple_map::result_b<1,1,0,0>::type range_t;
   range_t r = tm.find(Node_id(0), Node_id(3), any(), any());
   range_t::iterator qi = r.begin();
   triple_map::const_iterator i = tm.begin();

   std::cout << *i << std::endl;
   BOOST_CHECK( ! qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK( ! qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) ); //subject is not checked

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK( ! qi.predicate()(*i++) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef Map_triple<1,1,1,1> triple_map;
   triple_map tm;
   insert_seq(tm, t);

   triple_map::result_b<0,0,0,0>::type r1 = tm.find(any(), any(), any(), any());
   BOOST_CHECK_EQUAL( distance(r1), 7 );

   triple_map::result_b<1,0,0,0>::type r2 = tm.find(Node_id(0), any(), any(), any());
   BOOST_CHECK_EQUAL( distance(r2), 6 );

   triple_map::result_b<1,0,1,0>::type r3 = tm.find(Node_id(0), any(), Node_id(0), any());
   BOOST_CHECK_EQUAL( distance(r3), 5 );

   triple_map::result_b<0,0,0,1>::type r4 = tm.find(any(), any(), any(), Doc_id(1));
   BOOST_CHECK_EQUAL( distance(r4), 1 );

   triple_map::result_b<1,1,1,1>::type r5 = tm.find(Node_id(0), Node_id(2), Node_id(0), Doc_id(0));
   BOOST_CHECK_EQUAL( distance(r5), 1 );

   triple_map::result_b<1,0,1,0>::type r6 = tm.find(Node_id(0), any(), Node_id(2), any());
   BOOST_CHECK_EQUAL( distance(r6), 0 );

   triple_map::result_b<0,0,1,0>::type r7 = tm.find(any(), any(), Node_id(0), any());
   BOOST_CHECK_EQUAL( distance(r7), 6 );

   triple_map::result_b<1,1,0,0>::type r8 = tm.find(Node_id(0), Node_id(3), any(), any());
   BOOST_CHECK_EQUAL( distance(r8), 3 );
}

}//namespace test
}//namespace owlcpp
