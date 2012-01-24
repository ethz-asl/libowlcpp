/** @file "/owlcpp/lib/rdf/test/doc_info_map_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE doc_info_map_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"
#include "owlcpp/rdf/node_map_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

const std::string path1 = "path1";
const std::string path2 = "path2";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Doc_map dm;
   dm.insert(path1, Node_id(13), Node_id(0));
   BOOST_CHECK_THROW(
            dm.insert(path1, Node_id(12), Node_id(0)),
            Doc_map::Err
   );

   dm.insert(path2, Node_id(13), Node_id(1));
   Doc_map::iri_range ir1 = dm.find_iri(Node_id(13));
   BOOST_CHECK(ir1);
   const Doc_id id1 = ir1.front();
   BOOST_CHECK_EQUAL(Node_id(13), dm.iri(id1));

   //document versionIRI is returned by pointer
   BOOST_REQUIRE_MESSAGE(dm.version(id1), "versionIRI exists");
   BOOST_CHECK_EQUAL(Node_id(1), *dm.version(id1));
}

}//namespace test
}//namespace owlcpp
