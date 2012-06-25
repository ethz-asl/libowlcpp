/** @file "/owlcpp/lib/rdf/test/map_node_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_node_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_node.hpp"
#include "owlcpp/rdf/nodes_std.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_node mn(( Nodes_none() ));

   BOOST_CHECK_EQUAL(mn.size(), 0U);
   BOOST_CHECK_THROW(mn.at(t::T_owl_Class::id()), Rdf_err);

   const Node_id nid1 = mn.insert_iri(
            t::T_owl_Class::ns_type::id(), t::T_owl_Class::name()
   );
   BOOST_CHECK_NE(nid1, t::T_owl_Class::id());

   const Node_id nid2 = mn.insert_iri(Ns_id(42), "node2");
   Node const& n2 = mn.at(nid2);
   BOOST_CHECK_EQUAL(n2.value_str(), "node2");
   BOOST_CHECK( is_empty(mn.datatype(nid2)) );
   BOOST_CHECK(mn.language(nid2).empty());

   const Node_id nid3 = mn.insert_literal("some string", nid2, "en");
   BOOST_CHECK_EQUAL(mn[nid3].value_str(), "some string");
   BOOST_CHECK_EQUAL(mn.datatype(nid3), nid2);
   BOOST_CHECK_EQUAL(mn.language(nid3), "en");
}


/** OWL-aware node map
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Map_node mn(( Nodes_owl() ));
   BOOST_CHECK_EQUAL(mn.size(), 0U);

   Node const& node1 = mn.at(t::T_owl_Class::id());
   BOOST_CHECK_EQUAL(node1.ns_id(), t::N_owl::id());

   Ns_id const* nsid1 = mn.find(t::N_owl::id());

   const Node_id nid1 = mn.insert_iri(
            t::T_owl_Class::ns_type::id(), t::T_owl_Class::name()
   );
   BOOST_CHECK_EQUAL(mn.size(), 0U);
   BOOST_CHECK_EQUAL(nid1, t::T_owl_Class::id());
   BOOST_CHECK_THROW(
            mn.insert_iri(t::T_owl_Class::ns_type::id(), "some_name"),
            Map_node::Err
   );
}

/** Test blank nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Map_node mn;
   const Node_id nid2 = mn.insert_iri(Ns_id(42), "some_name");
   const Doc_id did1(13);
   const std::string b1 = "blank1";
   const Node_id nid3 = mn.insert_blank(did1, b1);
   BOOST_CHECK_EQUAL(mn.blank_node_doc(nid3), did1);
   BOOST_CHECK_THROW(mn.blank_node_doc(nid2), Map_node::Err);
   BOOST_REQUIRE( mn.find_blank(did1, b1) );
   BOOST_CHECK_EQUAL(*mn.find_blank(did1, b1), nid3);
   BOOST_CHECK( is_empty(mn.datatype(nid2)) );
   BOOST_CHECK(mn.language(nid2).empty());
}

/** Test literal nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Map_node mn;
   const Node_id nid2 = mn.insert_iri(Ns_id(42), "some_name");
   const Node_id nid4 = mn.insert_literal("some string", nid2, "en");
   BOOST_CHECK_EQUAL(mn.at(nid4).value_str(), "some string");
   BOOST_CHECK_EQUAL(mn.datatype(nid4), nid2);
   BOOST_CHECK_EQUAL(mn.language(nid4), "en");
   Node_id const*const nid4p = mn.find_literal("some string", nid2, "en");
   BOOST_REQUIRE(nid4p);
   BOOST_CHECK_EQUAL(*nid4p, nid4);
   BOOST_CHECK( ! mn.find_literal("some string", nid2, "fr") );
}

}//namespace test
}//namespace owlcpp
