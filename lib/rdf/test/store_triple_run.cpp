/** @file "/owlcpp/lib/rdf/test/store_triple_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE store_triple_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/store_triple.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string ns1 = "http://example.xyz/example1";
const std::string ns1p = "ex1";
const std::string ns2 = "http://example.xyz/example2";
const std::string ns2p = "ex2";

/** Test namespaces
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Store_triple ts(( Nodes_none() ));
   BOOST_CHECK_EQUAL(ts.namespaces().size(), 0U);
   const Ns_id nsid1 = ts.insert_ns(ns1);
   BOOST_CHECK_EQUAL(ts.namespaces().size(), 1U);
   BOOST_CHECK_EQUAL(ts[nsid1], ns1);
   ts.insert_prefix(nsid1, ns1p);
   BOOST_CHECK_EQUAL(ts.prefix(nsid1), ns1p);
   BOOST_REQUIRE( ts.find_prefix(ns1p) );
   BOOST_CHECK_EQUAL(*ts.find_prefix(ns1p), nsid1);
   BOOST_CHECK( ! ts.find_prefix(ns2p) );
}

/** Test nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Store_triple ts(( Nodes_none() ));

   const Node_id nid1 = ts.insert_iri_node(ni1);
   const Node_id nid1a = ts.insert_iri_node(ni1);
   BOOST_CHECK_EQUAL(nid1, nid1a);
   BOOST_CHECK_EQUAL(ts.string(nid1), ni1);
   const Node_id nid2 = ts.insert_iri_node(ni2);
   BOOST_CHECK_NE(nid1, nid2);

   //same namespace IRIs
   BOOST_CHECK_EQUAL(ts[nid1].ns_id(), ts[nid2].ns_id());
   //different fragment names
   BOOST_CHECK_NE(ts[nid1].value_str(), ts[nid2].value_str());

   const Node_id nid3 = ts.insert_iri_node(terms::N_owl::iri() + "#Ontology");
   BOOST_CHECK_NE(nid3, terms::T_owl_Ontology::id()); //non-standard ID
   ts.insert_iri_node(terms::N_owl::iri() + "#blah"); //inserting new node into standard namespace
}

/** OWL-aware triple store
BOOST_AUTO_TEST_CASE( case03 ) {
   Store_triple ts; //default constructor makes OWL-aware store

   //correct term
   const Node_id nid3 = ts.insert_iri_node(terms::N_owl::iri() + "#Ontology");
   BOOST_CHECK_EQUAL(nid3, terms::T_owl_Ontology::id());

   //misspelled term
   BOOST_CHECK_THROW(
            ts.insert_iri_node(terms::N_owl::iri() + "#Ontolog"),
            Store_triple::Err
   );

   //empty fragment
   BOOST_CHECK_THROW(
            ts.insert_iri_node(terms::N_owl::iri()),
            Store_triple::Err
   );
}
*******************************************************************************/

/** Empty IRI node maps to T_empty_::id() ID
BOOST_AUTO_TEST_CASE( case04 ) {
   Store_triple ts(Node_map_std::get(Nodes_none()));
   Node_id const* nid = ts.find_iri_node("");
   BOOST_REQUIRE(nid);
   BOOST_CHECK_EQUAL(*nid, terms::T_empty_::id());
}
*******************************************************************************/

/**
BOOST_AUTO_TEST_CASE( case05 ) {
   Store_triple ts;
   BOOST_CHECK_EQUAL(ts.documents().size(), 0U);
   const Doc_id did = ts.insert_doc("path", ni1, ni2).first;
   BOOST_REQUIRE_EQUAL( ts.documents().size(), 1u );
   BOOST_CHECK_EQUAL( *ts.documents().begin(), did );
   BOOST_CHECK_EQUAL( ts.path(did), "path" );
   const Node_id nid1 = ts.ontology_iri_id(did);
   Node const& node1 = ts.at(nid1);
   BOOST_CHECK_EQUAL( node1.value_str(), "n1" );
   BOOST_REQUIRE( ts.find_doc_iri(ni1) );
   BOOST_REQUIRE( ts.find_doc_iri(ni2) );
}
*******************************************************************************/

/**
BOOST_AUTO_TEST_CASE( case06 ) {
   Store_triple ts = sample_triples_01();
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
//   BOOST_ERROR("blah");
}
*******************************************************************************/

}//namespace test
}//namespace owlcpp
