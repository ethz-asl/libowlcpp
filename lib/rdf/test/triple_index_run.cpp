/** @file "/owlcpp/lib/rdf/test/triple_index_run.cpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE triple_index_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"

#include "owlcpp/rdf/detail/fragment_set.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"
#include "boost/fusion/container/generation/make_vector.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace m = map_triple_detail;

/** Test fragment set
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef m::Fragment_set<Doc_id,Node_id,Node_id> fs_t;
   fs_t fs;
   typedef fs_t::value_type val_t;
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(2)) );
   fs.insert( val_t(Doc_id(2), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(2), Node_id(2), Node_id(1)) );
   fs.insert( val_t(Doc_id(2), Node_id(1), Node_id(2)) );
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(1), Node_id(2), Node_id(1)) );
   BOOST_CHECK_EQUAL(fs.size(), 6U);
   typedef fs_t::const_range range_t;
   range_t r1 = fs.find(any(), any(), any());
   BOOST_CHECK_EQUAL(r1.size(), 6U);
   range_t r2 = fs.find(Doc_id(1), any(), any());
   BOOST_CHECK_EQUAL(r2.size(), 3U);
   range_t r3 = fs.find(Doc_id(1), Node_id(2), any());
   BOOST_CHECK_EQUAL(r3.size(), 1U);

   fs_t::result<Doc_id,any,Node_id>::type r4 = fs.find(Doc_id(2), any(), Node_id(1));
   BOOST_CHECK_EQUAL(distance(r4), 2);
}

}//namespace test
}//namespace owlcpp
