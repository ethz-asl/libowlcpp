/** @file "/owlcpp/lib/rdf/test/node_map_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_map_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/std_nodes.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/** Regular node map
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Node_map nm;

   BOOST_CHECK_EQUAL(nm.size(), 0U);
   BOOST_CHECK_THROW(nm.at(terms::T_owl_Class::id()), Rdf_err);
   const Node_id nid1 = nm.insert_iri(
            terms::T_owl_Class::ns_type::id(), terms::T_owl_Class::name()
   );
   BOOST_CHECK_NE(nid1, terms::T_owl_Class::id());

   const Node_id nid2 = nm.insert_iri(Ns_id(42), "node2");
   Node const& n2 = nm.at(nid2);
   BOOST_CHECK_EQUAL(n2.value_str(), "node2");
   BOOST_CHECK( is_empty(nm.datatype(nid2)) );
   BOOST_CHECK(nm.language(nid2).empty());

   const Node_id nid3 = nm.insert_literal("some string", nid2, "en");
   BOOST_CHECK_EQUAL(nm[nid3].value_str(), "some string");
   BOOST_CHECK_EQUAL(nm.datatype(nid3), nid2);
   BOOST_CHECK_EQUAL(nm.language(nid3), "en");
}

/** OWL-aware node map
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Node_map nm( Node_map_std::get(Nodes_owl()) );
   BOOST_CHECK_EQUAL(nm.size(), 0U);

   Node const& node1 = nm.at(terms::T_owl_Class::id());
   BOOST_CHECK_EQUAL(node1.ns_id(), terms::N_owl::id());

   Node_map::ns_range r1 = nm.find(terms::N_owl::id());

   const Node_id nid1 = nm.insert_iri(
            terms::T_owl_Class::ns_type::id(), terms::T_owl_Class::name()
   );
   BOOST_CHECK_EQUAL(nm.size(), 0U);
   BOOST_CHECK_EQUAL(nid1, terms::T_owl_Class::id());
   BOOST_CHECK_THROW(
            nm.insert_iri(terms::T_owl_Class::ns_type::id(), "some_name"),
            Node_map::Err
   );
}

/** Test blank nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Node_map nm;
   const Node_id nid2 = nm.insert_iri(Ns_id(42), "some_name");
   const Doc_id did1(13);
   const std::string b1 = "blank1";
   const Node_id nid3 = nm.insert_blank(did1, b1);
   BOOST_CHECK_EQUAL(nm.blank_node_doc(nid3), did1);
   BOOST_CHECK_THROW(nm.blank_node_doc(nid2), Node_map::Err);
   BOOST_REQUIRE( nm.find_blank(did1, b1) );
   BOOST_CHECK_EQUAL(*nm.find_blank(did1, b1), nid3);
   BOOST_CHECK( is_empty(nm.datatype(nid2)) );
   BOOST_CHECK(nm.language(nid2).empty());
}

/** Test literal nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Node_map nm;
   const Node_id nid2 = nm.insert_iri(Ns_id(42), "some_name");
   const Node_id nid4 = nm.insert_literal("some string", nid2, "en");
   BOOST_CHECK_EQUAL(nm.at(nid4).value_str(), "some string");
   BOOST_CHECK_EQUAL(nm.datatype(nid4), nid2);
   BOOST_CHECK_EQUAL(nm.language(nid4), "en");
   Node_id const*const nid4p = nm.find_literal("some string", nid2, "en");
   BOOST_REQUIRE(nid4p);
   BOOST_CHECK_EQUAL(*nid4p, nid4);
   BOOST_CHECK( ! nm.find_literal("some string", nid2, "fr") );
}

}//namespace test
}//namespace owlcpp
