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
namespace mpl = boost::mpl;
#include "test_utils.hpp"

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
BOOST_AUTO_TEST_CASE( test_iterator ) {
   typedef Map_triple<
            mpl::vector1<
               mpl::vector4<Subj_tag, Pred_tag, Obj_tag, Doc_tag>
            >
   > map_triple;
   map_triple mt;
   BOOST_CHECK_EQUAL(mt.size(), 0U);
   BOOST_CHECK(mt.begin() == mt.end());
   insert_seq(mt, t);
   BOOST_CHECK_EQUAL(mt.size(), 7U);
   BOOST_CHECK(mt.begin() != mt.end());

}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_index_subject_search ) {
   typedef Map_triple<
            mpl::vector1<
               mpl::vector4<Subj_tag, Pred_tag, Obj_tag, Doc_tag>
            >
   > map_triple;
   map_triple mt;
   BOOST_CHECK_EQUAL(mt.size(), 0U);
   insert_seq(mt, t);
   BOOST_CHECK_EQUAL(mt.size(), 7U);
   map_triple::query_b<1,1,0,0>::range r1 = mt.find(Node_id(2), Node_id(3), any, any);
   BOOST_CHECK(!r1);
   map_triple::query_b<1,1,0,0>::range r2 = mt.find(Node_id(0), Node_id(1), any, any);
   BOOST_CHECK(r2);
   BOOST_CHECK_EQUAL(boost::distance(r2), 1);
   map_triple::query_b<0,1,0,0>::range r3 = mt.find(any, Node_id(3), any, any);
   BOOST_CHECK_EQUAL(boost::distance(r3), 4);

   BOOST_CHECK_THROW( mt.erase(triple(0,13,0,1)), Rdf_err );
   mt.erase(triple(0,3,0,1));
   BOOST_CHECK_EQUAL(mt.size(), 6U);

   mt.clear();
   BOOST_CHECK_EQUAL(mt.size(), 0U);
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_index_predicate_search ) {
   typedef Map_triple<
            mpl::vector1<
               mpl::vector4<Pred_tag, Subj_tag, Obj_tag, Doc_tag>
            >
   > map_triple;
   map_triple mt;
   insert_seq(mt, t);
   typedef map_triple::query_b<0,1,0,0>::range range_t;
   range_t r = mt.find(any, Node_id(3), any, any);
   BOOST_CHECK(r);
   BOOST_CHECK_EQUAL(distance(r), 4U);
}

}//namespace test
}//namespace owlcpp
