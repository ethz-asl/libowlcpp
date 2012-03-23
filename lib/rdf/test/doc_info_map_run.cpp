/** @file "/owlcpp/lib/rdf/test/doc_info_map_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE doc_info_map_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = "path1";
const std::string path2 = "path2";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Doc_map dm;

   //empty ontology IRI
   BOOST_CHECK_THROW(
            dm.insert(path1, Node_id(), Node_id()),
            Doc_map::Err
   );

   //empty versionIRI
   BOOST_CHECK_NO_THROW(
            dm.insert(path1, Node_id(13), Node_id())
   );

   //same path, different IRI
   BOOST_CHECK_THROW(
            dm.insert(path1, Node_id(12), Node_id(0)),
            Doc_map::Err
   );

   //empty path
   std::pair<Doc_id, bool> p1 = dm.insert("", Node_id(13), Node_id());
   BOOST_CHECK(p1.second);
   std::pair<Doc_id, bool> p2 = dm.insert("", Node_id(13), Node_id());
   BOOST_CHECK(! p2.second);
   BOOST_CHECK_EQUAL(p1.first, p2.first);

   std::pair<Doc_id, bool> p3 = dm.insert("", Node_id(12), Node_id());
   BOOST_CHECK(p3.second);
   BOOST_CHECK_NE(p1.first, p3.first);

   const Node_id nid1(13);
   const Node_id nid2(42);
   dm.insert(path2, nid1, nid2);
   Doc_map::iri_range ir1 = dm.find_iri(nid1);
   BOOST_CHECK(ir1);
   const Doc_id id1 = ir1.front();
   BOOST_CHECK_EQUAL(nid1, dm.ontology_iri(id1));

   //document versionIRI is returned by pointer
   BOOST_REQUIRE_MESSAGE(dm.version_iri(id1) != terms::T_empty_::id(), "versionIRI exists");
   BOOST_CHECK_EQUAL(nid2, dm.version_iri(id1));
}

}//namespace test
}//namespace owlcpp
