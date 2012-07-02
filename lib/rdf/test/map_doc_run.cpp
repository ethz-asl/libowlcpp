/** @file "/owlcpp/lib/rdf/test/map_doc_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_doc_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_doc.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;

const std::string path1 = "path1";
const std::string path2 = "path2";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_doc dm;

   //empty ontology IRI
   BOOST_CHECK_THROW(
            dm.insert(t::T_empty_::id(), path1, Node_id(42)),
            Map_doc::Err
   );

   dm.insert(Node_id(42), path1, t::T_empty_::id());

   //same path, different IRI
   BOOST_CHECK_THROW(
            dm.insert(Node_id(12), path1, t::T_empty_::id()),
            Map_doc::Err
   );

   //empty path
   std::pair<Doc_id, bool> p1 = dm.insert(Node_id(13));
   BOOST_CHECK(p1.second);
   std::pair<Doc_id, bool> p2 = dm.insert(Node_id(13));
   BOOST_CHECK(! p2.second);
   BOOST_CHECK_EQUAL(p1.first, p2.first);

   std::pair<Doc_id, bool> p3 = dm.insert(Node_id(12));
   BOOST_CHECK(p3.second);
   BOOST_CHECK_NE(p1.first, p3.first);

   const Node_id nid1(13);
   const Node_id nid2(42);
   dm.insert(nid1, path2, nid2);
   Map_doc::iri_range ir1 = dm.find_iri(nid1);
   BOOST_CHECK(ir1);
   const Doc_id id1 = ir1.front();
   BOOST_CHECK_EQUAL(nid1, dm[id1].ontology_iri());

   //document versionIRI is returned by pointer
   BOOST_REQUIRE_MESSAGE(dm[id1].version_iri() != terms::T_empty_::id(), "versionIRI exists");
   BOOST_CHECK_EQUAL(nid2, dm[id1].version_iri());

}

}//namespace test
}//namespace owlcpp
