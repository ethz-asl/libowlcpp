/** @file "/owlcpp/lib/rdf/test/node_map_owl_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_map_owl_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_map_owl.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string n1 = "name1";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Node_map_owl nmo;

   BOOST_CHECK_NO_THROW(nmo.at(terms::T_owl_Ontology::id()));

   const Ns_id iid1 = terms::N_rdf::id();
   BOOST_CHECK_THROW( nmo.insert_iri(iid1, n1), Node_map_owl::Err);

   const Ns_id iid2 = Ns_id(42);
   BOOST_REQUIRE( ! Owl_terms::is_owl(iid2) );
   const Node_id id1 = nmo.insert_iri(iid2, n1);
   BOOST_CHECK( ! Owl_terms::is_owl(id1) );
}

}//namespace test
}//namespace owlcpp
