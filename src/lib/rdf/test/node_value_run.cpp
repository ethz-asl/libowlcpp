/** @file "/owlcpp/lib/rdf/test/node_value_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_value_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

/**@test Literal bool nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_bool_node ) {
   BOOST_CHECK_THROW(Node_bool("falsee"), Rdf_err);
   BOOST_CHECK(   Node_bool("true").value() );
   BOOST_CHECK( ! Node_bool("false").value() );
   BOOST_CHECK(   Node_bool("1").value() );
   BOOST_CHECK( ! Node_bool("0").value() );
   BOOST_CHECK(   Node_bool(true).value() );
   BOOST_CHECK( ! Node_bool(false).value() );
   BOOST_CHECK_THROW(Node_bool(42), Rdf_err);
   BOOST_CHECK_THROW(Node_bool(-1), Rdf_err);
   BOOST_CHECK_THROW(Node_bool("42"), Rdf_err);
   BOOST_CHECK(   Node_bool(1).value() );
   BOOST_CHECK( ! Node_bool(0).value() );
}

/**@test Literal int nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_int_node ) {
   BOOST_CHECK_THROW(Node_int("blah"), Rdf_err);
   BOOST_CHECK_EQUAL(Node_int("-1").value(), -1);

   BOOST_CHECK_EQUAL(Node_int(0.9).value(), 0);
   BOOST_CHECK_THROW(Node_int("0.9"), Rdf_err);
   BOOST_CHECK_EQUAL(Node_int(1e10).value(), 1e10);
   BOOST_CHECK_THROW(Node_int("1e10"), Rdf_err);
}

/**@test Literal unsigned nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_unsigned_node ) {
   BOOST_CHECK_THROW(Node_unsigned(-1), Rdf_err);
   BOOST_CHECK_THROW(Node_unsigned("-1"), Rdf_err);

   BOOST_CHECK_EQUAL(Node_unsigned(1e10).value(), 1e10);
}

/**@test Literal real nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_real_node ) {
   BOOST_CHECK_THROW(Node_double("blah"), Rdf_err);
   BOOST_CHECK_EQUAL(Node_double("-1").value(), -1.0);

   BOOST_CHECK_EQUAL(Node_double(0.9).value(), 0.9);
   BOOST_CHECK_EQUAL(Node_double("-0.9").value(), -0.9);
   BOOST_CHECK_EQUAL(Node_double(1e10).value(), 1e10);
   BOOST_CHECK_EQUAL(Node_double("1e10").value(), 1e10);
}


/**@test white space collapsing for booleans and decimals
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_whitespace_collapsing ) {
   BOOST_CHECK(  Node_bool("true\r\r").value() );
   BOOST_CHECK_EQUAL(Node_int("   42").value(), 42);
   BOOST_CHECK_EQUAL(Node_double("\t   42\n\r").value(), 42.0);
}

}//namespace test
}//namespace owlcpp
