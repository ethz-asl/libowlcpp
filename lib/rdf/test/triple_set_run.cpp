/** @file "/owlcpp/lib/rdf/test/triple_set_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2014
*******************************************************************************/
#define BOOST_TEST_MODULE triple_set_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/test_utils.hpp"

#include "owlcpp/rdf/detail/triple_set.hpp"
#include "owlcpp/rdf/triple_tags.hpp"

namespace owlcpp{ namespace test{

namespace m = map_triple_detail;

/** Test triple set
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef m::Triple_set<Pred_tag, Subj_tag, Obj_tag> ts_t;
   ts_t ts;
   BOOST_CHECK(ts.empty());
   ts.insert( triple(1, 1, 2, 0) );
   insert_seq(ts, random_triples1);
   BOOST_CHECK_EQUAL(ts.size(), 21U);
   ts.erase(triple(1, 1, 2, 0));
   BOOST_CHECK_EQUAL(ts.size(), 20U);
   BOOST_CHECK_THROW(
            ts.erase(triple(1, 1, 2, 0)),
            Rdf_err
   );

   BOOST_CHECK_EQUAL(ts[0], triple(1,0,6,4));
   BOOST_CHECK_EQUAL(ts[10], triple(0,4,8,2));

   BOOST_CHECK_EQUAL(
            ts.find(
                     Node_id(5), //pred
                     any, //subj
                     any //obj
            ).size(),
            4
   );

   BOOST_CHECK_EQUAL(
            distance(
                     ts.find(
                              Node_id(1), //pred
                              any, //subj
                              Node_id(0) //obj
                     )
            ),
            2
   );

   BOOST_CHECK_EQUAL(
            ts.find(
                     Node_id(3), //pred
                     Node_id(6), //subj
                     any //obj
            ).size(),
            2
   );
}

}//namespace test
}//namespace owlcpp
