/** @file "/owlcpp/lib/rdf/test/node_map_std_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_map_std_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_map_std.hpp"
#include "owlcpp/rdf/std_nodes.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Node_map_std const& nms0 = Node_map_std::get(Nodes_none());
   Node_map_std const& nms1 = Node_map_std::get(Nodes_owl());

   BOOST_CHECK_EQUAL(nms0.ns_id_next()(), terms::N_blank::index + 1 );
   BOOST_CHECK_EQUAL(nms0.node_id_next()(), terms::T_empty_::index + 1);

   BOOST_CHECK_EQUAL(nms1.ns_id_next()(), terms::N_owl::index + 1 );
   BOOST_CHECK_EQUAL(nms1.node_id_next()(), terms::T_xsd_positiveInteger::index + 1);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Node_map_std const& nms0 = Node_map_std::get(Nodes_none());
   Node_map_std const& nms1 = Node_map_std::get(Nodes_owl());
   Node_map_std const& nms2 = Node_map_std::get(Nodes_owl());
   BOOST_CHECK_NE(&nms0, &nms1); //different instances
   BOOST_CHECK_EQUAL(&nms1, &nms2); //same instance
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Node_map_std const& nms = Node_map_std::get(Nodes_owl());

   BOOST_CHECK( nms.have(terms::T_xsd_positiveInteger::id()));

   BOOST_CHECK_THROW(nms.at(Node_id(3000)), Rdf_err);
   BOOST_CHECK_THROW(nms.at(Node_id(3)), Rdf_err);
   BOOST_CHECK_NO_THROW(nms.at(terms::T_empty_::id()));
   BOOST_CHECK_NO_THROW(nms.at(terms::T_xsd_ENTITY::id()));
   BOOST_CHECK_NO_THROW(nms.at(terms::T_xsd_positiveInteger::id()));

   BOOST_REQUIRE( nms.find(Node(terms::T_empty_())) );
   BOOST_REQUIRE( nms.find(Node(terms::T_owl_Class())) );
   BOOST_REQUIRE( nms.find(Node(terms::T_owl_targetValue())) );
   BOOST_REQUIRE( nms.find(Node(terms::T_rdfs_Datatype())) );
   BOOST_REQUIRE( nms.find(Node(terms::T_rdf_type())) );
   BOOST_REQUIRE( nms.find(Node(terms::T_xsd_dateTime())) );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_empty_())).front(),
            terms::T_empty_::id()
   );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_owl_Class())).front(),
            terms::T_owl_Class::id()
   );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_owl_targetValue())).front(),
            terms::T_owl_targetValue::id()
   );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_rdfs_Datatype())).front(),
            terms::T_rdfs_Datatype::id()
   );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_rdf_type())).front(),
            terms::T_rdf_type::id()
   );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_xsd_dateTime())).front(),
            terms::T_xsd_dateTime::id()
   );

   BOOST_CHECK_EQUAL(
            nms.find(Node(terms::T_xsd_positiveInteger())).front(),
            terms::T_xsd_positiveInteger::id()
   );
}

}//namespace test
}//namespace owlcpp
