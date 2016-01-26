/** @file "/owlcpp/lib/rdf/test/triple_store_erase_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2015
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_erase_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "boost/foreach.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_triples.hpp"

namespace owlcpp{ namespace test{

namespace t = owlcpp::terms;

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_docs_01 ) {
   Triple_store ts = sample_triples_01();
   BOOST_CHECK_EQUAL(ts.map_node().size(), 10U);
   BOOST_CHECK_EQUAL(ts.map_triple().size(), 7U);
   Node_id const* nid1 = ts.find_node_iri(iri11);
   BOOST_REQUIRE(nid1);
   Triple_store::query_b<1,0,0,0>::range q =
            ts.find_triple(*nid1, any, any, any);
   BOOST_REQUIRE(q);
   Triple const& t = q.front();
   ts.erase(t);
   BOOST_CHECK_EQUAL(ts.map_triple().size(), 6U);
}

}//namespace test
}//namespace owlcpp
