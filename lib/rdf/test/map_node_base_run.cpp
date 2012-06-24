/** @file "/owlcpp/lib/rdf/test/map_node_base_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_node_base_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/detail/map_node_base.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace d = owlcpp::detail;
namespace t = owlcpp::terms;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   d::Map_node_base mnb(( Node_id() ));
   BOOST_CHECK( ! mnb.have(Node_id(42)) );

   const Node_id id1 = mnb.insert_iri(Ns_id(42), "blah");
   BOOST_CHECK( mnb.have(id1) );

   const Node_id id2 = mnb.insert_blank(42, Doc_id(5));
   BOOST_CHECK( mnb.have(id2) );

   const Node_id id3 = mnb.insert_literal("blah", Node_id(5), "jp");
   BOOST_CHECK( mnb.have(id3) );

   BOOST_CHECK_NE(id1, id2);
   BOOST_CHECK_NE(id1, id3);

   std::auto_ptr<Node> p1 = mnb.remove(id1);
   BOOST_CHECK( ! mnb.find(*p1) );

   const Node_id id4 = mnb.insert_literal("blahblah", Node_id(5), "jp");
   BOOST_CHECK( mnb.have(id4) );
   BOOST_CHECK_EQUAL( id1, id4 );

   const Node_id id5 = mnb.insert_iri(Ns_id(42), "blah");
   BOOST_CHECK( mnb.have(id5) );
   BOOST_CHECK_NE(id1, id5);

   //insert existing node with new ID
   BOOST_CHECK_THROW(
            mnb.insert_iri(Node_id(42), Ns_id(42), "blah"),
            Rdf_err
   );

   //insert new node with existing ID
   BOOST_CHECK_THROW(
            mnb.insert_iri(id5, Ns_id(142), "new_node"),
            Rdf_err
   );

   //insert existing node with its current ID (NOP)
   mnb.insert_iri(id5, Ns_id(42), "blah");

   mnb.insert_iri(Node_id(42), Ns_id(142), "new_node");
}

BOOST_AUTO_TEST_CASE( case02 ) {
   d::Map_node_base mnb(( Node_id() ));
   BOOST_CHECK( mnb.empty() );
   mnb.insert_iri(
            t::T_owl_AsymmetricProperty::id(),
            t::T_owl_AsymmetricProperty::ns_type::id(),
            t::T_owl_AsymmetricProperty::name()
   );
   BOOST_CHECK( ! mnb.empty() );
   const Node_id id1 = t::T_owl_AsymmetricProperty::id();
   BOOST_CHECK( mnb.have(id1) );
   BOOST_CHECK_EQUAL(
            mnb.at(id1).value_str(),
            t::T_owl_AsymmetricProperty::name()
   );
}

}//namespace test
}//namespace owlcpp
