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
//   BOOST_CHECK_EQUAL(nid1, terms::T_owl_Class::id());

//   BOOST_CHECK_EQUAL( nm[terms::T_rdfs_range::id()], Node(terms::T_rdfs_range()) );
}

}//namespace test
}//namespace owlcpp
