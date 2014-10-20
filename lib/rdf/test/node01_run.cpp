/** @file "/owlcpp/lib/rdf/test/node01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node01_run
#include <iostream>
#include <vector>
#include <memory>
#include "boost/assign/std/vector.hpp"
#include "boost/foreach.hpp"
#include "boost/test/unit_test.hpp"
#include "boost/range/algorithm/sort.hpp"
#include "boost/range/algorithm/unique.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

/**@test IRI nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_iri_node ) {
   const Node_iri n1(Ns_id(42), "blah");
   const Node_iri n2(Ns_id(41), "blah");
   BOOST_CHECK_NE(n1, n2);

   BOOST_CHECK_THROW(Node_iri(owlcpp::terms::blank::id(), "blah"), Rdf_err);

   const Node_iri n3(Ns_id(42), "blahblah");
   BOOST_CHECK_NE(n1,  n3);
   BOOST_CHECK_NE(n2,  n3);

   const Node_iri n4(Ns_id(42), "blah");
   BOOST_CHECK_EQUAL(n1,  n4);

   const Node_iri n5(owlcpp::terms::empty::id(), "");
   const Node_iri n6;
   BOOST_CHECK_EQUAL(n6.ns_id(), owlcpp::terms::empty::id());
   BOOST_CHECK_EQUAL(n6.fragment(), "");
   BOOST_CHECK_EQUAL(n5,  n6);

   const Node_iri n7(owlcpp::terms::empty::id(), "blah");
   BOOST_CHECK_NE(n5,  n7);
}

/**@test clone IRI nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_clone_iri_node ) {
   const Node_iri n1(Ns_id(42), "blah");
   std::auto_ptr<Node> pn1(n1.clone());
   BOOST_CHECK_EQUAL(n1, *pn1);

   const Node_iri n5(owlcpp::terms::empty::id(), "");
   std::auto_ptr<Node> pn5(n5.clone());
   BOOST_CHECK_EQUAL(n5, *pn5);
}


/**@test Blank nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_blank_node ) {
   const Node_blank n1(0, Doc_id(42));
   BOOST_CHECK_EQUAL(n1.ns_id(), owlcpp::terms::blank::id());
   const Node_blank n2(0, Doc_id(42));
   BOOST_CHECK_EQUAL(n1,  n2);

   const Node_blank n3(1, Doc_id(42));
   BOOST_CHECK_NE(n1,  n3);

   const Node_blank n4(0, Doc_id(41));
   BOOST_CHECK_NE(n1,  n4);
}

/**@test Compare nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_compare ) {
   const Node_iri n1(Ns_id(42), "blah");
   const Node_blank n2(1, Doc_id(42));
   BOOST_CHECK_NE(n1,  n2);

   const Node_string n3("blah", Node_id(42));
   BOOST_CHECK_NE(n1,  n3);

   const Node_string n4("");
   BOOST_CHECK_NE(n1,  n4);

   const Node_iri n5;
   BOOST_CHECK_NE(n5,  n4);
}

/**@test node hash
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_hash ) {
   const Node_string n1("blah", Node_id(42), "en");
   const Node_string n2("blah", Node_id(42), "fr");
   const Node_string n3("blah", Node_id(42), "en");
   const Node_iri n4(Ns_id(42), "blah");
   const Node_blank n5(0, Doc_id(42));
   const Node_blank n6(42, Doc_id(0));
   const Node_blank n7(1, Doc_id(42));
   const Node_string n8("", Node_id(0));
   const Node_blank n9(0, Doc_id(0));

   std::vector<std::size_t> h;
   std::vector<Node const*> v;
   using namespace boost::assign;
   v += &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9;
   BOOST_CHECK_EQUAL(hash_value(*v[0]), hash_value(n1));
   BOOST_CHECK_EQUAL(hash_value(*v[0]), hash_value(*v[2]));

   BOOST_FOREACH(Node const* n, v) {
      h.push_back(hash_value(*n));
   }
   BOOST_CHECK_EQUAL(boost::unique(boost::sort(h)).size(), 8);
}

}//namespace test
}//namespace owlcpp
