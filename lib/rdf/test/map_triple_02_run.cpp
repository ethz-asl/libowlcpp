/** @file "/owlcpp/lib/rdf/test/map_triple_02_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_triple_02_run
#include "boost/test/unit_test.hpp"
#include "boost/range.hpp"
#include "test/exception_fixture.hpp"
#include <iostream>

#include "owlcpp/rdf/map_triple.hpp"
#include "rdf/test/test_utils.hpp"
namespace mpl = boost::mpl;

namespace owlcpp{ namespace test{

const unsigned t[][4] = {
         {0,1,0,0},
         {0,2,0,0},
         {0,3,0,0},
         {1,3,0,0},
         {0,3,1,0},
         {0,3,0,1},
         {0,4,0,0},
};

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_triple_01 ) {
   typedef Map_triple<
            mpl::vector4<
               mpl::vector4<Subj_tag, Pred_tag, Obj_tag, Doc_tag>,
               mpl::vector4<Pred_tag, Subj_tag, Obj_tag, Doc_tag>,
               mpl::vector4<Obj_tag, Subj_tag, Pred_tag, Doc_tag>,
               mpl::vector4<Doc_tag, Pred_tag, Subj_tag, Obj_tag>
            >
   > map_triple;
   map_triple tm;
   insert_seq(tm, t);

   map_triple::query_b<0,0,0,0>::range r1 = tm.find(any, any, any, any);
   BOOST_CHECK_EQUAL( boost::distance(r1), 7 ); //cannot use size

   map_triple::query_b<1,0,0,0>::range r2 = tm.find(Node_id(0), any, any, any);
   BOOST_CHECK_EQUAL( r2.size(), 6 );

   map_triple::query_b<1,0,1,0>::range r3 = tm.find(Node_id(0), any, Node_id(0), any);
   BOOST_CHECK_EQUAL( r3.size(), 5 );

   map_triple::query_b<0,0,0,1>::range r4 = tm.find(any, any, any, Doc_id(1));
   BOOST_CHECK_EQUAL( r4.size(), 1 );

   map_triple::query_b<1,1,1,1>::range r5 = tm.find(Node_id(0), Node_id(2), Node_id(0), Doc_id(0));
   BOOST_CHECK_EQUAL( r5.size(), 1 );

   map_triple::query_b<1,0,1,0>::range r6 = tm.find(Node_id(0), any, Node_id(2), any);
   BOOST_CHECK_EQUAL( r6.size(), 0 );

   map_triple::query_b<0,0,1,0>::range r7 = tm.find(any, any, Node_id(0), any);
   BOOST_CHECK_EQUAL( r7.size(), 6 );

   map_triple::query_b<1,1,0,0>::range r8 = tm.find(Node_id(0), Node_id(3), any, any);
   BOOST_CHECK_EQUAL( r8.size(), 3 );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_triple_02 ) {
   typedef Map_triple<> map_triple;
   map_triple mt;
   insert_seq(mt, random_triples1);
   BOOST_CHECK_EQUAL(mt.size(), 20U);

   BOOST_CHECK_EQUAL(mt.find(Node_id(6), Node_id(3), any, any).size(), 2);
}

}//namespace test
}//namespace owlcpp
