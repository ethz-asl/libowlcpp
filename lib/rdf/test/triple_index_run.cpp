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

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef m::Fragment_set<Doc_id,Node_id,Node_id> fs_t;
   fs_t fs;
   typedef fs_t::value_type val_t;
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(2)) );
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(1), Node_id(2), Node_id(1)) );
   BOOST_CHECK_EQUAL(fs.size(), 3U);
   typedef fs_t::const_range range_t;
   range_t r = fs.find(boost::fusion::make_vector(any(), Node_id(1), any()));
   BOOST_CHECK_EQUAL(r.size(), 2U);
}

}//namespace test
}//namespace owlcpp
