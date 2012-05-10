/** @file "/owlcpp/lib/rdf/test/triple_index_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_index_run
#include "boost/test/unit_test.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/detail/triple_index.hpp"

namespace owlcpp{ namespace test{

using namespace query_detail;

BOOST_GLOBAL_FIXTURE( Exception_fixture );

typedef boost::ptr_vector<Triple> triple_ptrv_t;

/**@brief
*******************************************************************************/
template<class Map, class Seq> triple_ptrv_t insert_triples(Map& m, Seq const& s) {
   triple_ptrv_t v;
   for(int i = 0; i != boost::size(s); ++i) {
      Triple* t = new Triple(
               Node_id(s[i][0]), Node_id(s[i][1]), Node_id(s[i][2]), Doc_id(s[i][3])
               );
      v.push_back(t);
      m.insert(*t);
   }
   return v;
}

const unsigned t1[][4] = {
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
   typedef Index_config<0,0,0,0> ic;
   BOOST_CHECK_EQUAL(ic::n_indexed::value, 0);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef Index_config<1,0,0,1> ic;
   BOOST_CHECK_EQUAL(ic::n_indexed::value, 2);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 0>::type::value), 0);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 1>::type::value), 1);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 2>::type::value), 1);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 3>::type::value), 1);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef Index_config<1,1,0,1> ic;
   BOOST_CHECK_EQUAL(ic::n_indexed::value, 3);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 0>::type::value), 0);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 1>::type::value), 1);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 2>::type::value), 2);
   BOOST_CHECK_EQUAL((mpl::at_c<ic::position, 3>::type::value), 2);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case10 ) {
   Triple_index<1,1,0,0> ti;
   triple_ptrv_t v = insert_triples(ti, t1);
   BOOST_CHECK(ti.find<0>(0));
}

}//namespace test
}//namespace owlcpp
