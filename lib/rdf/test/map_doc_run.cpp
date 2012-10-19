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
const std::string path3 = "path3";
const std::string path4 = "path4";
const Node_id nid1(12);
const Node_id nid2(13);
const Node_id nid3(42);

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_doc md;

   //empty ontology IRI
   BOOST_CHECK_THROW(
            md.insert(t::T_empty_::id(), path1, nid1),
            Map_doc::Err
   );

   md.insert(nid3, path1, t::T_empty_::id());

   //same path, different IRI
   BOOST_CHECK_THROW(
            md.insert(nid1, path1, t::T_empty_::id()),
            Map_doc::Err
   );

   //empty path
   std::pair<Doc_id, bool> p1 = md.insert(nid2);
   BOOST_CHECK(p1.second);
   std::pair<Doc_id, bool> p2 = md.insert(nid2);
   BOOST_CHECK(! p2.second);
   BOOST_CHECK_EQUAL(p1.first, p2.first);

   std::pair<Doc_id, bool> p3 = md.insert(nid1);
   BOOST_CHECK(p3.second);
   BOOST_CHECK_NE(p1.first, p3.first);

   md.insert(nid2, path2, nid3);
   Map_doc::iri_range ir1 = md.find_iri(nid2);
   BOOST_CHECK(ir1);
   const Doc_id id1 = ir1.front();
   BOOST_CHECK_EQUAL(nid2, md[id1].ontology_iri);

   //document versionIRI is returned by pointer
   BOOST_REQUIRE_MESSAGE(md[id1].version_iri != terms::T_empty_::id(), "versionIRI exists");
   BOOST_CHECK_EQUAL(nid3, md[id1].version_iri);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_search ) {
   Map_doc md;
   md.insert(nid1, path1, nid2);
   md.insert(nid1, path2, nid3);
   BOOST_CHECK_EQUAL(distance(md.find_iri(nid1)), 2);
   BOOST_CHECK( ! md.find_iri(nid2) );

   BOOST_CHECK( ! md.find_version(nid1) );
   BOOST_CHECK_EQUAL(distance(md.find_version(nid2)), 1);
}

}//namespace test
}//namespace owlcpp
