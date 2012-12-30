/** @file "/owlcpp/lib/rdf/test/map_id_object_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_id_object_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/detail/map_id_object.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/node_iri.hpp"

namespace owlcpp{ namespace test{
namespace d = owlcpp::detail;

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   d::Map_id_object<Node_iri, Node_id> mio(Node_id(0));
}

}//namespace test
}//namespace owlcpp
