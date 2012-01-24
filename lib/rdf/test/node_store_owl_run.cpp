/** @file "/owlcpp/lib/rdf/test/node_store_owl_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_store_owl_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/node_map_owl.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Node_map_owl nm;

   BOOST_CHECK_GT(nm.size(), 50u);

   const Node_id nid1 = nm.insert( Node( terms::T_owl_Class() ) );
   BOOST_CHECK_EQUAL(nid1, terms::T_owl_Class::id());

   BOOST_CHECK_EQUAL( nm[terms::T_rdfs_range::id()], Node(terms::T_rdfs_range()) );
}

}//namespace test
}//namespace owlcpp
