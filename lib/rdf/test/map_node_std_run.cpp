/** @file "/owlcpp/lib/rdf/test/map_node_std_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_node_std_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_node_std.hpp"
#include "owlcpp/rdf/std_nodes.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_node_std const& nms0 = Map_node_std::get(Nodes_none());
   Map_node_std const& nms1 = Map_node_std::get(Nodes_owl());

   BOOST_CHECK_EQUAL(nms0.ns_id_next()(), terms::N_blank::index + 1 );
   BOOST_CHECK_EQUAL(nms0.node_id_next()(), terms::T_empty_::index + 1);

   BOOST_CHECK_EQUAL(nms1.ns_id_next()(), terms::N_owl::index + 1 );
   BOOST_CHECK_EQUAL(nms1.node_id_next()(), terms::T_xsd_positiveInteger::index + 1);
}

}//namespace test
}//namespace owlcpp
