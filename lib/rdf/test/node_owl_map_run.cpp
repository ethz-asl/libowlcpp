/** @file "/owlcpp/lib/rdf/test/node_owl_map_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_owl_map_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_owl_map.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   BOOST_CHECK( ! is_owl(Ns_id()));
   BOOST_CHECK( ! is_owl(terms::N_blank::id()));
   BOOST_CHECK( ! is_owl(terms::N_empty::id()));
   BOOST_CHECK( is_owl(terms::N_owl::id()));
   BOOST_CHECK( is_owl(terms::N_rdfs::id()));

   BOOST_CHECK( Node_owl_map::get().is_owl(terms::T_xsd_positiveInteger::id()));

   Node_owl_map const& nom = Node_owl_map::get();
   BOOST_CHECK_THROW(nom.at(Node_id(3000)), Rdf_err);
   BOOST_CHECK_THROW(nom.at(Node_id(3)), Rdf_err);
   BOOST_CHECK_NO_THROW(nom.at(terms::T_empty_::id()));
   BOOST_CHECK_NO_THROW(nom.at(terms::T_xsd_ENTITY::id()));
   BOOST_CHECK_NO_THROW(nom.at(terms::T_xsd_positiveInteger::id()));

   BOOST_REQUIRE( nom.find(Node(terms::T_empty_())) );
   BOOST_REQUIRE( nom.find(Node(terms::T_owl_Class())) );
   BOOST_REQUIRE( nom.find(Node(terms::T_owl_targetValue())) );
   BOOST_REQUIRE( nom.find(Node(terms::T_rdfs_Datatype())) );
   BOOST_REQUIRE( nom.find(Node(terms::T_rdf_type())) );
   BOOST_REQUIRE( nom.find(Node(terms::T_xsd_dateTime())) );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_empty_())).front(),
            terms::T_empty_::id()
   );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_owl_Class())).front(),
            terms::T_owl_Class::id()
   );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_owl_targetValue())).front(),
            terms::T_owl_targetValue::id()
   );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_rdfs_Datatype())).front(),
            terms::T_rdfs_Datatype::id()
   );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_rdf_type())).front(),
            terms::T_rdf_type::id()
   );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_xsd_dateTime())).front(),
            terms::T_xsd_dateTime::id()
   );

   BOOST_CHECK_EQUAL(
            nom.find(Node(terms::T_xsd_positiveInteger())).front(),
            terms::T_xsd_positiveInteger::id()
   );
}

}//namespace test
}//namespace owlcpp
