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
   Node_map_std const& nms0 = Node_map_std::get(Insert_none());
   Node_map_std const& nms1 = Node_map_std::get(Nodes_owl());
   Node_map_std const& nms2 = Node_map_std::get(Nodes_system());

   BOOST_CHECK_EQUAL(nms0.ns_id_next(), Ns_id(0));
   BOOST_CHECK_EQUAL(nms0.node_id_next(), Node_id(0));

   BOOST_CHECK_EQUAL(nms1.ns_id_next()(), terms::N_owl::index + 1 );
   BOOST_CHECK_EQUAL(nms1.node_id_next()(), terms::T_xsd_positiveInteger::index + 1);

   BOOST_CHECK_EQUAL(nms2.ns_id_next()(), terms::N_blank::index + 1 );
   BOOST_CHECK_EQUAL(nms2.node_id_next()(), terms::T_empty_::index + 1);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Node_map_std const& nms0 = Node_map_std::get(Insert_none());
   Node_map_std const& nms1 = Node_map_std::get(Nodes_owl());
   Node_map_std const& nms2 = Node_map_std::get(Nodes_owl());
   BOOST_CHECK_NE(&nms0, &nms1); //different instances
   BOOST_CHECK_EQUAL(&nms1, &nms2); //same instance
}

}//namespace test
}//namespace owlcpp
