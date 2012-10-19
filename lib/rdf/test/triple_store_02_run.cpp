/** @file "/owlcpp/lib/rdf/test/triple_store_02_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_02_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
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

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_docs ) {
   Triple_store ts;
   BOOST_CHECK_EQUAL(ts.map_doc().size(), 0U);
   const Node_id nid1 = ts.insert_node_iri(ni1);
   const Node_id nid2 = ts.insert_node_iri(ni2);
   const Doc_id did1 = ts.insert_doc(nid1, path1, nid2).first;
   BOOST_REQUIRE_EQUAL( ts.map_doc().size(), 1u );
   BOOST_CHECK_EQUAL( *ts.map_doc().begin(), did1 );

   BOOST_CHECK_EQUAL( ts.at(did1).path, path1 );
   const Node_id nid1a = ts[did1].ontology_iri;
   BOOST_CHECK_EQUAL( nid1, nid1a );

   const Doc_id did2 = ts.insert_doc(ni3, path2, ni4).first;
   BOOST_CHECK_EQUAL( ts.map_doc().size(), 2u );
   const Node_id nid3 = ts[did2].ontology_iri;
   BOOST_CHECK_NE( to_string(ts[nid3]).find("node3"), std::string::npos ); //same value
   BOOST_CHECK_EQUAL( ts[ts[nid3].ns_id()], ns2 ); //same namespace IRI
   const Node_id nid4 = ts[did2].version_iri;
   BOOST_CHECK_NE( to_string(ts[nid4]).find("node4"), std::string::npos );
   BOOST_CHECK_EQUAL( ts[ts[nid4].ns_id()], ns2 );

   Triple_store::doc_iri_range r1 = ts.map_doc().find_iri(nid3);
   BOOST_REQUIRE_EQUAL(boost::distance(r1), 1);

   Node const& node1 = ts.at(nid1);
   BOOST_CHECK_NE( to_string(node1).find("node1"), std::string::npos );
   BOOST_CHECK(   ts.find_doc_iri(ni1) );
   BOOST_CHECK(   ts.find_doc_iri(ni3) );
   BOOST_CHECK( ! ts.find_doc_version(ni3) );
   BOOST_CHECK(   ts.find_doc_version(ni4) );
}

/** Test blank nodes, OWL-aware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_blank_nodes_owl ) {
   Triple_store ts;
   BOOST_CHECK_EQUAL(ts.map_doc().size(), 0U);
   const Doc_id did1 = ts.insert_doc(ni1, path1, ni2).first;
   BOOST_CHECK_EQUAL(ts.map_doc().size(), 1U);

   Node_id const* nid1 = ts.find_node_iri(ni1);
   BOOST_REQUIRE(nid1);
   Node_id const* nid2 = ts.find_node_iri(ni2);
   BOOST_REQUIRE(nid2);

   const Node_id nid3 = ts.insert_blank(0, did1);
   Node_id const*const nid3a = ts.find_blank(0, did1);
   BOOST_REQUIRE(nid3a);
   BOOST_CHECK_EQUAL(*nid3a, nid3);
   const Node_id nid3b = ts.insert_blank(0, did1);
   BOOST_CHECK_EQUAL(nid3b, nid3);

   const Node_id nid4 = ts.insert_blank(1, did1);
   BOOST_CHECK_NE(nid4, nid3);

   const Doc_id did2 = ts.insert_doc(ni3, path2).first;
   const Node_id nid5 = ts.insert_blank(0, did2);
   BOOST_CHECK_NE(nid5, nid3);
   BOOST_CHECK_NE(nid5, nid4);
}

/** Test literal nodes, OWL-aware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_nodes_owl ) {
   Triple_store ts;
   const Node_id nid1 = ts.insert_node_iri(ni1);
   const Node_id nid2 = ts.insert_literal("blah", nid1);
   const Node_id nid2a = ts.insert_literal("blah", ni1);
   BOOST_CHECK_EQUAL(nid2, nid2a);

   const Node_id nid3 = ts.insert_literal("blah");
   BOOST_CHECK_NE(nid2, nid3);
   const Node_id nid3a = ts.insert_literal("blah", t::T_empty_::id());
   BOOST_CHECK_EQUAL(nid3, nid3a);

   const Node_id nid3b = ts.insert_literal("blah", "", "");
   BOOST_CHECK_EQUAL(nid3, nid3b);

   const Node_id nid4 = ts.insert_literal("blah", "", "en");
   BOOST_CHECK_NE(nid3, nid4);

   const Node_id nid5 = ts.insert_literal("blah", "", "fr");
   BOOST_CHECK_NE(nid4, nid5);
   BOOST_CHECK_NE(nid3, nid5);
}
/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_print_triples ) {
/*
   Triple_store ts = sample_triples_01();
   BOOST_FOREACH(Triple const& t, ts.triples()) {
      std::cout
      << '\"'
      << to_string_short(t.subject(), ts) << "\"\t\""
      << to_string_short(t.predicate(), ts) << "\"\t\""
      << to_string_short(t.object(), ts) << "\"\t\n"
      ;
      Node const& node = ts.at(t.subject());
      const Ns_id nsid1 = node.ns_id();
      const std::string iri = ts[nsid1];
      const std::string pref = ts.iris().prefix(nsid1);
   }
*/
//   BOOST_ERROR("blah");
}

}//namespace test
}//namespace owlcpp
