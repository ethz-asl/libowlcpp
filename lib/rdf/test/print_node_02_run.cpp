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

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;
const std::string ns1 = "http://example.xyz/example1";
const std::string ns1p = "ex1";
const std::string ns2 = "http://example.xyz/example2";
const std::string ns2p = "ex2";
const std::string ni1 = ns1 + "#node1";
const std::string ni2 = ns1 + "#node2";
const std::string ni3 = ns2 + "#node3";
const std::string ni4 = ns2 + "#node4";
const std::string path1 = "path1";
const std::string path2 = "path2";

/** Print nodes with triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;
   const Ns_id nsid1 = ts.insert_ns(ns1);
   const Ns_id nsid2 = ts.insert_ns(ns2);
   ts.insert_prefix(nsid1, ns1p);
   const Node_id nid1 = ts.insert_node_iri(ni1);
   const Node_id nid2 = ts.insert_node_iri(ni3);
   BOOST_CHECK_EQUAL(ts[nsid1], ns1);
   BOOST_CHECK_EQUAL(     to_string(nid1, ts), ni1);
   BOOST_CHECK_EQUAL(to_string_pref(nid1, ts), ni1);
   BOOST_CHECK_EQUAL(to_string_full(nid1, ts), ni1);
   BOOST_CHECK_EQUAL(     to_string(nid2, ts), ni1);
   BOOST_CHECK_EQUAL(to_string_pref(nid2, ts), ni1);
   BOOST_CHECK_EQUAL(to_string_full(nid2, ts), ni1);
}

}//namespace test
}//namespace owlcpp

