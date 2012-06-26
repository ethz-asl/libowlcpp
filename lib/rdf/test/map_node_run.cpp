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

   const Node_id nid3 = mn.insert_literal("some string", nid2, "en");
   Node const& node3 = mn[nid3];
   BOOST_CHECK_EQUAL(node3.value_str(), "some string");
   BOOST_REQUIRE(typeid(node3) == typeid(Node_literal));
   Node_literal const& node3l = static_cast<Node_literal const&>(node3);
   BOOST_CHECK_EQUAL(node3l.datatype(), nid2);
   BOOST_CHECK_EQUAL(node3l.language(), "en");
}


/** OWL-aware node map
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Map_node mn(( Nodes_owl() ));
   BOOST_CHECK_EQUAL(mn.size(), 0U);

   Node const& node1 = mn.at(t::T_owl_Class::id());
   BOOST_CHECK_EQUAL(node1.ns_id(), t::N_owl::id());

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

}//namespace test
}//namespace owlcpp
