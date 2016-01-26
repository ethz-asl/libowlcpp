/** @file "/owlcpp/lib/rdf/test/print_node_02_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE print_node_02_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "test/sample_triples.hpp"

namespace owlcpp{ namespace test{

namespace t = owlcpp::terms;

/**@test Print nodes with triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_print_node_02 ) {
   Triple_store ts;
   const Ns_id nsid1 = ts.insert(ns1);
   /*const Ns_id nsid2 =*/ ts.insert(ns2);
   ts.insert_prefix(nsid1, ns1p);
   const Node_id nid1 = ts.insert_node_iri(iri11);
   const Node_id nid2 = ts.insert_node_iri(iri23);

   //namespace IRI string
   BOOST_CHECK_EQUAL(ts[nsid1], ns1);
   BOOST_CHECK_EQUAL(ts.prefix(nsid1), ns1p);

   //prefix is defined for ns1, to_string() uses it
   BOOST_CHECK_EQUAL(to_string(nid1, ts), "ex1:node1");

   //prefix is defined for ns1, to_string() uses it
   BOOST_CHECK_EQUAL(to_string_pref(nid1, ts), "ex1:node1");

   //full namespace IRI is produced
   BOOST_CHECK_EQUAL(to_string_full(nid1, ts), iri11);

   //prefix is not defined, full namespace IRI is used
   BOOST_CHECK_EQUAL(to_string(nid2, ts), iri23);

   //prefix is not defined, it is generated
   const std::string str2 = to_string_pref(nid2, ts);
   BOOST_CHECK_EQUAL(str2.find("Ns"), 0U);
   BOOST_CHECK_NE(str2.find(":node3"), std::string::npos);

   //full namespace IRI is used
   BOOST_CHECK_EQUAL(to_string_full(nid2, ts), iri23);
}

}//namespace test
}//namespace owlcpp

