/** @file "/owlcpp/lib/rdf/test/node01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node01_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/** IRI nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   const Node_iri n1(Ns_id(42), "blah");
   const Node_iri n2(Ns_id(41), "blah");
   BOOST_CHECK(n1 != n2);

   BOOST_CHECK_THROW(Node_iri(owlcpp::terms::N_blank::id(), "blah"), Rdf_err);

   const Node_iri n3(Ns_id(42), "blahblah");
   BOOST_CHECK(n1 != n3);
   BOOST_CHECK(n2 != n3);

   const Node_iri n4(Ns_id(42), "blah");
   BOOST_CHECK(n1 == n4);
   BOOST_CHECK_EQUAL(Node_iri::make_hash(Ns_id(42), "blah"), n4.hash());

   const Node_iri n5(owlcpp::terms::N_empty::id(), "");
   BOOST_CHECK(n5.empty());
   const Node_iri n6;
   BOOST_CHECK(n6.empty());
   BOOST_CHECK(n5 == n6);

   const Node_iri n7(owlcpp::terms::N_empty::id(), "blah");
   BOOST_CHECK(n5 != n7);
}

/** Literal nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   const Node_literal n1("blah", Node_id(42), "en");
   BOOST_CHECK_EQUAL(n1.ns_id(), owlcpp::terms::N_empty::id());
   const Node_literal n2("blah", Node_id(42), "en");
   BOOST_CHECK(n1 == n2);

   const Node_literal n3("blahh", Node_id(42), "en");
   BOOST_CHECK(n1 != n3);
   BOOST_CHECK_EQUAL(Node_literal::make_hash("blahh", Node_id(42), "en"), n3.hash());

   const Node_literal n4("blah", Node_id(41), "en");
   BOOST_CHECK(n1 != n4);

   const Node_literal n5("blah", Node_id(42), "fr");
   BOOST_CHECK(n1 != n5);
}

/** Blank nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   const Node_blank n1(0, Doc_id(42));
   BOOST_CHECK_EQUAL(n1.ns_id(), owlcpp::terms::N_blank::id());
   const Node_blank n2(0, Doc_id(42));
   BOOST_CHECK(n1 == n2);

   const Node_blank n3(1, Doc_id(42));
   BOOST_CHECK(n1 != n3);

   const Node_blank n4(0, Doc_id(41));
   BOOST_CHECK(n1 != n4);
}

/** Compare nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   const Node_iri n1(Ns_id(42), "blah");
   const Node_blank n2(1, Doc_id(42));
   BOOST_CHECK(n1 != n2);

   const Node_literal n3("blah", Node_id(42));
   BOOST_CHECK(n1 != n3);

   const Node_literal n4("");
   BOOST_CHECK(n1 != n4);

   const Node_iri n5;
   BOOST_CHECK(n5 != n4);
}

}//namespace test
}//namespace owlcpp
