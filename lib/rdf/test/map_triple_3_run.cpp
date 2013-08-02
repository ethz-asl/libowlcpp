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

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const unsigned seq1[][4] = {
      // random     Subj,Obj,Pred,Doc  Pred,Subj,Obj,Doc:
         {1,5,9,4}, //  0  5  3  1     1  0  6  4
         {6,3,4,4}, //  0  4  8  2     5  0  2  7
         {8,5,0,7}, //  1  1  0  9     1  1  0  9
         {3,2,9,5}, //  1  0  6  4     2  1  0  1
         {8,5,2,7}, //  1  5  9  4     6  1  7  0
         {2,1,0,1}, //  2  1  0  1     3  2  9  5
         {3,4,5,9}, //  2  9  0  5     5  2  4  6
         {0,4,8,2}, //  3  4  5  9     7  2  8  7
         {9,8,6,6}, //  3  2  9  5     6  3  1  3
         {1,1,0,9}, //  4  6  6  2     6  3  4  4
         {6,1,7,0}, //  5  0  2  7     0  4  8  2
         {4,6,6,2}, //  5  2  4  6     3  4  5  9
         {2,9,0,5}, //  5  8  4  9     0  5  3  1
         {1,0,6,4}, //  6  3  1  3     1  5  9  4
         {5,2,4,6}, //  6  3  4  4     8  5  0  7
         {7,2,8,7}, //  6  1  7  0     8  5  2  7
         {0,5,3,1}, //  7  2  8  7     4  6  6  2
         {6,3,1,3}, //  8  5  0  7     5  8  4  9
         {5,8,4,9}, //  8  5  2  7     9  8  6  6
         {5,0,2,7}, //  9  8  6  6     2  9  0  5
};

/**@test inserting triples
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_triple<> mt;
   BOOST_CHECK_EQUAL(mt.size(), 0U);

   mt.insert(triple(0,0,0,0));
   BOOST_CHECK_EQUAL(mt.size(), 1U);

   insert_seq(mt, seq1);
   BOOST_CHECK_EQUAL(mt.size(), 21U);
}

/**@test search query type
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Map_triple<> mt;

}

}//namespace test
}//namespace owlcpp
