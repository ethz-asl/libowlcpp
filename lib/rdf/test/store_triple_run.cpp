/** @file "/owlcpp/lib/rdf/test/store_triple_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE store_triple_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/store_triple.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
//#include "owlcpp/rdf/query_node.hpp"

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

   const Node_id nid1 = ts.insert_node_iri(ni1);
   const Node_id nid1a = ts.insert_node_iri(ni1);
   BOOST_CHECK_EQUAL(nid1, nid1a);
//   BOOST_CHECK_EQUAL(to_string(nid1, ts), ni1);
   const Node_id nid2 = ts.insert_node_iri(ni2);
   BOOST_CHECK_NE(nid1, nid2);

   //same namespace IRIs
   BOOST_CHECK_EQUAL(ts[nid1].ns_id(), ts[nid2].ns_id());
   //different fragment names
   BOOST_CHECK_NE(ts[nid1].value_str(), ts[nid2].value_str());

   const Node_id nid3 = ts.insert_node_iri(t::N_owl::iri() + "#Ontology");
   BOOST_CHECK_NE(nid3, t::T_owl_Ontology::id()); //non-standard ID
   ts.insert_node_iri(t::N_owl::iri() + "#blah"); //inserting new node into standard namespace
}

/** OWL-aware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Store_triple ts; //default constructor makes OWL-aware store

   BOOST_CHECK_EQUAL(
            ts.at(t::T_owl_Ontology::id()).value_str(),
            t::T_owl_Ontology::name()
   );

   //correct term
   const Node_id nid1 = ts.insert_node_iri(t::N_owl::id(), t::T_owl_Ontology::name());
   BOOST_CHECK_EQUAL(nid1, t::T_owl_Ontology::id());

   const Node_id nid2 = ts.insert_node_iri(t::N_owl::iri() + "#Ontology");
   const Ns_id nsid2 = ts.at(nid2).ns_id();
   BOOST_CHECK_EQUAL(nsid2, t::N_owl::id());
   BOOST_CHECK_EQUAL(nid2, t::T_owl_Ontology::id());

   //misspelled term
   BOOST_CHECK_THROW(
            ts.insert_node_iri(t::N_owl::iri() + "#Ontolog"),
            Store_triple::Err
   );

   //empty fragment
   BOOST_CHECK_THROW(
            ts.insert_node_iri(t::N_owl::iri()),
            Store_triple::Err
   );
}

/** Empty IRI node maps to T_empty_::id() ID
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Store_triple ts(( Nodes_none() ));
   const Node_id nid = ts.insert_node_iri("");
   BOOST_CHECK_EQUAL(nid, t::T_empty_::id());
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case05 ) {
   Store_triple ts;
   BOOST_CHECK_EQUAL(ts.docs().size(), 0U);
   const Node_id nid1 = ts.insert_node_iri(ni1);
   const Node_id nid2 = ts.insert_node_iri(ni2);
   const Doc_id did1 = ts.insert_doc(nid1, path1, nid2).first;
   BOOST_REQUIRE_EQUAL( ts.docs().size(), 1u );
   BOOST_CHECK_EQUAL( *ts.docs().begin(), did1 );

   BOOST_CHECK_EQUAL( ts.at(did1).path(), path1 );
   const Node_id nid1a = ts[did1].ontology_iri();
   BOOST_CHECK_EQUAL( nid1, nid1a );

   const Doc_id did2 = ts.insert_doc(ni3, path2, ni4).first;
   BOOST_CHECK_EQUAL( ts.docs().size(), 2u );
   const Node_id nid3 = ts[did2].ontology_iri();
   BOOST_CHECK_EQUAL( ts[nid3].value_str(), "node3" ); //same value
   BOOST_CHECK_EQUAL( ts[ts[nid3].ns_id()], ns2 ); //same namespace IRI
   const Node_id nid4 = ts[did2].version_iri();
   BOOST_CHECK_EQUAL( ts[nid4].value_str(), "node4" );
   BOOST_CHECK_EQUAL( ts[ts[nid4].ns_id()], ns2 );

   Store_triple::doc_iri_range r1 = ts.find_doc_iri(nid3);
   BOOST_REQUIRE_EQUAL(boost::distance(r1), 1);
   BOOST_CHECK_EQUAL()

/*
   Node const& node1 = ts.at(nid1);
   BOOST_CHECK_EQUAL( node1.value_str(), "n1" );
   BOOST_REQUIRE( ts.find_doc_iri(ni1) );
   BOOST_REQUIRE( ts.find_doc_iri(ni2) );
*/
}

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
