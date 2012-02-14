/** @file "/owlcpp/lib/rdf/test/owl_terms_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE owl_terms_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/owl_terms.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   BOOST_CHECK( ! Owl_terms::is_owl(Ns_id()));
   BOOST_CHECK( ! Owl_terms::is_owl(terms::N_blank::id()));
   BOOST_CHECK( ! Owl_terms::is_owl(terms::N_empty::id()));
   BOOST_CHECK( Owl_terms::is_owl(terms::N_owl::id()));
   BOOST_CHECK( Owl_terms::is_owl(terms::N_rdfs::id()));

   BOOST_CHECK( Owl_terms::is_owl(terms::T_xsd_positiveInteger::id()));

   Owl_terms const& ot = Owl_terms::get();
   BOOST_CHECK_THROW(ot.at(Node_id(3000)), Owl_terms::Err);
   BOOST_CHECK_THROW(ot.at(Node_id(3)), Owl_terms::Err);
   BOOST_CHECK_NO_THROW(ot.at(terms::T_empty_::id()));
   BOOST_CHECK_NO_THROW(ot.at(terms::T_xsd_ENTITY::id()));
   BOOST_CHECK_NO_THROW(ot.at(terms::T_xsd_positiveInteger::id()));

   BOOST_REQUIRE( ot.find(Node(terms::T_empty_())) );
   BOOST_REQUIRE( ot.find(Node(terms::T_owl_Class())) );
   BOOST_REQUIRE( ot.find(Node(terms::T_owl_targetValue())) );
   BOOST_REQUIRE( ot.find(Node(terms::T_rdfs_Datatype())) );
   BOOST_REQUIRE( ot.find(Node(terms::T_rdf_type())) );
   BOOST_REQUIRE( ot.find(Node(terms::T_xsd_dateTime())) );

   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_empty_())), terms::T_empty_::id() );
   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_owl_Class())), terms::T_owl_Class::id() );
   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_owl_targetValue())), terms::T_owl_targetValue::id() );
   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_rdfs_Datatype())), terms::T_rdfs_Datatype::id() );
   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_rdf_type())), terms::T_rdf_type::id() );
   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_xsd_dateTime())), terms::T_xsd_dateTime::id() );
   BOOST_CHECK_EQUAL( *ot.find(Node(terms::T_xsd_positiveInteger())), terms::T_xsd_positiveInteger::id() );
}

}//namespace test
}//namespace owlcpp
