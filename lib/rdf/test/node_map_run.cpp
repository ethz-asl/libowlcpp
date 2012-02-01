/** @file "/owlcpp/lib/rdf/test/node_map_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_map_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Node_map nm;

   BOOST_CHECK_EQUAL(nm.size(), 1u);

   const Node_id nid1 = nm.insert_iri(
            terms::T_owl_Class::ns_type::id(), terms::T_owl_Class::name()
   );

   const Node_id nid2 = nm.insert_iri(Ns_id(42), "node2");
   BOOST_CHECK_EQUAL(nm[nid2].value_str(), "node2");
   BOOST_CHECK( ! nm.datatype(nid2));
   BOOST_CHECK(nm.language(nid2).empty());

   const Node_id nid3 = nm.insert_literal("some string", nid2, "en");
   BOOST_CHECK_EQUAL(nm[nid3].value_str(), "some string");
   BOOST_REQUIRE(nm.datatype(nid3));
   BOOST_CHECK_EQUAL(*nm.datatype(nid3), nid2);
   BOOST_CHECK_EQUAL(nm.language(nid3), "en");
}

}//namespace test
}//namespace owlcpp
