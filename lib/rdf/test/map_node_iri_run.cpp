/** @file "/owlcpp/lib/rdf/test/map_node_iri_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_node_iri_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_node_iri.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_node_iri mnb(( Node_id() ));
   BOOST_CHECK( ! mnb.have(Node_id(42)) );

   const Node_id id1 = mnb.insert(Ns_id(42), "blah");
   BOOST_REQUIRE( mnb.have(id1) );

   const Node_id id2 = mnb.insert(Ns_id(42), "blah");
   BOOST_CHECK( mnb.have(id2) );

   BOOST_CHECK_EQUAL(id1, id2);

   const Node_id id3 = mnb.insert(Ns_id(43), "blah");
   BOOST_CHECK( mnb.have(id3) );

   BOOST_CHECK_NE(id1, id3);

   const Node_iri node1 = mnb[id1];
   mnb.remove(id1);
   BOOST_CHECK( ! mnb.find(node1) );
   BOOST_CHECK( ! mnb.find(id1) );

   const Node_id id4 = mnb.insert(Ns_id(13), "blahblah");
   BOOST_CHECK( mnb.have(id4) );
   BOOST_CHECK_EQUAL(id1, id4); //ID got recycled

   //insert existing node with new ID
   BOOST_CHECK_THROW(
            mnb.insert(Node_id(42), Ns_id(43), "blah"),
            Rdf_err
   );

   //insert new node with existing ID
   BOOST_CHECK_THROW(
            mnb.insert(id4, Ns_id(142), "new_node"),
            Rdf_err
   );

   //insert existing node with its current ID (NOP)
   mnb.insert(id4, Ns_id(13), "blahblah");

   BOOST_CHECK( ! mnb.have(Node_id(42)) );
   mnb.insert(Node_id(42), Ns_id(142), "new_node");
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   const Node_id id0(( 13 ));
   Map_node_iri mnb(id0);

   const Node_id id_1(( 10 ));
   BOOST_CHECK_THROW( mnb.insert(id_1, Ns_id(43), "blah"), Rdf_err );

   mnb.insert(id0, Ns_id(43), "blah");

}
}//namespace test
}//namespace owlcpp
