/** @file "/owlcpp/lib/rdf/test/node_string_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_string_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/hash_node.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;

/**@test Literal string nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_string_node ) {
   const Node_string n1("blah", Node_id(42), "en");
   BOOST_CHECK_EQUAL(n1.ns_id(), owlcpp::terms::empty::id());
   const Node_string n2("blah", Node_id(42), "en");
   BOOST_CHECK_EQUAL(n1,  n2);
   BOOST_CHECK_EQUAL(n1.hash(), n2.hash());

   const Node_string n3("blahh", Node_id(42), "en");
   BOOST_CHECK_NE(n1,  n3);
   BOOST_CHECK_NE(n1.hash(), n3.hash());

   const Node_string n4("blah", Node_id(41), "en");
   BOOST_CHECK_NE(n1,  n4);
   BOOST_CHECK_NE(n1.hash(), n4.hash());

   const Node_string n5("blah", Node_id(42), "fr");
   BOOST_CHECK_NE(n1,  n5);
   BOOST_CHECK_NE(n1.hash(), n5.hash());
}

/**@test Compare nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_compare_nodes ) {
   const Node_string n1a("blah", "en");
   const Node_string n1b("blah", t::xsd_string::id(), "en");
   BOOST_CHECK_EQUAL(n1a, n1b);

   const Node_string n2a("blah");
   const Node_string n2b("blah", t::xsd_string::id());
   BOOST_CHECK_EQUAL(n2a, n2b);
}

}//namespace test
}//namespace owlcpp
