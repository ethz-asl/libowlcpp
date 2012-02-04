/** @file "/owlcpp/lib/rdf/test/triple_store_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string ni1 = "http://example.xyz/example#n1";
const std::string ni2 = "http://example.xyz/example#n2";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;

   const Node_id nid1 = ts.insert_iri_node(ni1);
   const Node_id nid1a = ts.insert_iri_node(ni1);
   BOOST_CHECK_EQUAL(nid1, nid1a);
   const Node_id nid2 = ts.insert_iri_node(ni2);
   BOOST_CHECK_NE(nid1, nid2);

   //same namespace IRIs
   BOOST_CHECK_EQUAL(ts[nid1].ns_id(), ts[nid2].ns_id());
   //different fragment names
   BOOST_CHECK_NE(ts[nid1].value_str(), ts[nid2].value_str());
}

/** Test inserting new nodes into OWL namespace
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Triple_store ts;

   //correct term
   BOOST_CHECK_NO_THROW( ts.insert_iri_node(terms::N_owl::iri() + "#Ontology") );

   //misspelled term
   BOOST_CHECK_THROW(
            ts.insert_iri_node(terms::N_owl::iri() + "#Ontolog"),
            Triple_store::Err
   );

   //empty fragment
   BOOST_CHECK_THROW(
            ts.insert_iri_node(terms::N_owl::iri()),
            Triple_store::Err
   );
}

/** Empty IRI node maps to T_empty_::id() ID
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Triple_store ts;
   Node_id const* nid = ts.find_iri_node("");
   BOOST_REQUIRE(nid);
   BOOST_CHECK_EQUAL(*nid, terms::T_empty_::id());
}

}//namespace test
}//namespace owlcpp
