/** @file "/owlcpp/lib/rdf/test/map_node_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_node_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_node.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_node mnb(( Node_id() ));
   BOOST_CHECK( ! mnb.valid(Node_id(42)) );

   const Node_id id1 = mnb.insert_iri(Ns_id(42), "blah");
   BOOST_CHECK( mnb.valid(id1) );

   const Node_id id2 = mnb.insert_blank(42, Doc_id(5));
   BOOST_CHECK( mnb.valid(id2) );

   const Node_id id3 = mnb.insert_literal("blah", Node_id(5), "en");
   BOOST_CHECK( mnb.valid(id3) );

   BOOST_CHECK_NE(id1, id2);
   BOOST_CHECK_NE(id1, id3);

   std::auto_ptr<Node> p1 = mnb.remove(id1);
   BOOST_CHECK( ! mnb.find(*p1) );

   const Node_id id4 = mnb.insert_literal("blahblah", Node_id(5), "fr");
   BOOST_CHECK( mnb.valid(id4) );
   BOOST_CHECK_EQUAL( id1, id4 );

   const Node_id id5 = mnb.insert_iri(Ns_id(42), "blah");
   BOOST_CHECK( mnb.valid(id5) );
   BOOST_CHECK_NE(id1, id5);
}

}//namespace test
}//namespace owlcpp
