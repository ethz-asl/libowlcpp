/** @file "/owlcpp/lib/rdf/test/node_map_std_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE node_map_std_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_map_std.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

struct Empty {
   static Ns_id fill(Iri_map_base& map) {return Ns_id(0);}
   static Node_id fill(Node_map_base& map) {return Node_id(0);}
};


/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Node_map_std const& nms = Node_map_std::get(Empty());
}

}//namespace test
}//namespace owlcpp
