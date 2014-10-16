/** @file "/owlcpp/lib/rdf/test/triple_store_01_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_01_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "test/sample_triples.hpp"

namespace owlcpp{ namespace test{

namespace t = owlcpp::terms;

/**@test Test namespaces, OWL-unaware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_namespaces ) {
   Triple_store ts(( Nodes_none() ));
   BOOST_CHECK(   ts.find(t::empty::id()));
   BOOST_CHECK( ! ts.find(t::owl::id()));
}

/**@test Test namespaces, OWL-aware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_namespaces_owl ) {
   Triple_store ts;
   BOOST_CHECK(ts.find(t::empty::id()));
   BOOST_CHECK(ts.find(t::owl::id()));

   BOOST_CHECK_EQUAL(ts.map_ns().size(), 0U);
   const Ns_id nsid1 = ts.insert(ns1);
   BOOST_CHECK_EQUAL(ts.map_ns().size(), 1U);
   BOOST_CHECK_EQUAL(ts[nsid1], ns1);
   ts.insert_prefix(nsid1, ns1p);
   BOOST_CHECK_EQUAL(ts.prefix(nsid1), ns1p);
   BOOST_REQUIRE( ts.find_prefix(ns1p) );
   BOOST_CHECK_EQUAL(*ts.find_prefix(ns1p), nsid1);
   BOOST_CHECK( ! ts.find_prefix(ns2p) );
}

/**@test Test find IRI nodes, OWL-unaware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_find_iri_nodes ) {
   Triple_store ts(( Nodes_none() ));
   BOOST_CHECK( ! ts.find_node_iri("some_random_name") );
   BOOST_CHECK( ! ts.find_node_iri(to_string_full(t::owl_Axiom())) );
}

/**@test Test IRI nodes, OWL-unaware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_iri_nodes ) {
   Triple_store ts(( Nodes_none() ));

   BOOST_CHECK_EQUAL(ts.map_node().size(), 0U);
   const Node_id nid1 = ts.insert_node_iri(iri11);
   BOOST_CHECK_EQUAL(ts.map_node().size(), 1U);
   const Node_id nid1a = ts.insert_node_iri(iri11);
   BOOST_CHECK_EQUAL(ts.map_node().size(), 1U);
   BOOST_CHECK_EQUAL(nid1, nid1a);
   BOOST_CHECK_EQUAL(to_string(nid1, ts), iri11);
   const Node_id nid2 = ts.insert_node_iri(iri12);
   BOOST_CHECK_NE(nid1, nid2);
   BOOST_CHECK_EQUAL(ts.map_node().size(), 2U);

   //same namespace IRIs
   BOOST_CHECK_EQUAL(ts[nid1].ns_id(), ts[nid2].ns_id());
   //different fragment names
   BOOST_CHECK_NE(to_string(nid1, ts), to_string(nid2, ts));

   const Node_id nid3 = ts.insert_node_iri(t::owl::iri() + "#Ontology");
   BOOST_CHECK_EQUAL(ts.map_node().size(), 3U);
   BOOST_CHECK_NE(nid3, t::owl_Ontology::id()); //non-standard ID
   ts.insert_node_iri(t::owl::iri() + "#blah"); //inserting new node into standard namespace
   BOOST_CHECK_EQUAL(ts.map_node().size(), 4U);
}

/**@test Test find IRI nodes, OWL-aware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_find_iri_nodes_owl ) {
   Triple_store ts;
   BOOST_CHECK( ! ts.find_node_iri("some_random_name") );
   BOOST_CHECK(   ts.find_node_iri(to_string_full(t::owl_Axiom())) );
}

/**@test Test IRI nodes, OWL-aware triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_nodes_owl ) {
   Triple_store ts;

   BOOST_CHECK_EQUAL(
            to_string_full(ts.at(t::owl_Ontology::id()), ts),
            to_string_full(t::owl_Ontology())
   );

   //correct term
   const Node_id nid1 = ts.insert_node_iri(
            t::owl::id(), t::owl_Ontology::fragment()
   );
   BOOST_CHECK_EQUAL(nid1, t::owl_Ontology::id());

   const Node_id nid2 = ts.insert_node_iri(t::owl::iri() + "#Ontology");
   const Ns_id nsid2 = ts.at(nid2).ns_id();
   BOOST_CHECK_EQUAL(nsid2, t::owl::id());
   BOOST_CHECK_EQUAL(nid2, t::owl_Ontology::id());

   //misspelled term
   BOOST_CHECK_THROW(
            ts.insert_node_iri(t::owl::iri() + "#Ontolog"),
            Triple_store::Err
   );

   //empty fragment
   BOOST_CHECK_THROW(
            ts.insert_node_iri(t::owl::iri()),
            Triple_store::Err
   );
}

/**@test Empty IRI node maps to empty_::id() ID
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_empty_iri ) {
   Triple_store ts1(( Nodes_none() ));
   Node_id nid = ts1.insert_node_iri("");
   BOOST_CHECK_EQUAL(nid, t::empty_::id());

   Triple_store ts2;
   nid = ts1.insert_node_iri("");
   BOOST_CHECK_EQUAL(nid, t::empty_::id());
}

}//namespace test
}//namespace owlcpp
