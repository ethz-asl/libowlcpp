/** @file "/owlcpp/lib/rdf/test/map_doc_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_doc_run
#include "boost/test/unit_test.hpp"
#include "boost/foreach.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_doc.hpp"

namespace owlcpp{ namespace test{

namespace t = owlcpp::terms;

const std::string path1 = "path1";
const std::string path2 = "path2";
const std::string path3 = "path3";
const std::string path4 = "path4";
const Node_id nid1(12);
const Node_id nid2(13);
const Node_id nid3(42);
const Node_id nid4(41);

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_doc ) {
   Map_doc md;
   BOOST_CHECK_EQUAL(md.size(), 0U);

   //empty ontology IRI
   BOOST_CHECK_THROW(
            md.insert(t::empty_::id(), path1, nid1),
            Map_doc::Err
   );
   BOOST_CHECK_EQUAL(md.size(), 0U);

   const Doc_id did1 = md.insert(nid3, path1, t::empty_::id()).first;
   BOOST_CHECK_EQUAL(md.size(), 1U);
   BOOST_CHECK_EQUAL(*md.begin(), did1);
   const std::pair<Doc_id,bool> p = md.insert(nid3, path1);
   BOOST_CHECK( ! p.second);
   BOOST_CHECK_EQUAL(p.first, did1);

   //same path, different IRI
   BOOST_CHECK_THROW(
            md.insert(nid1, path1, t::empty_::id()),
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

   const Doc_id did2 = md.insert(nid2, path2, nid3).first;
   Map_doc::iri_range ir1 = md.find_iri(nid2);
   BOOST_CHECK(ir1);
   const Doc_id did2a = ir1.front();
   BOOST_CHECK_EQUAL(did2a, did2);
   BOOST_CHECK_EQUAL(nid2, md[did2a].ontology_iri);

   //document versionIRI is returned by pointer
   BOOST_REQUIRE_MESSAGE(md[did2a].version_iri != terms::empty_::id(), "versionIRI exists");
   BOOST_CHECK_EQUAL(nid3, md[did2a].version_iri);

   BOOST_FOREACH(const Doc_id did, md) {
      std::cout << did << '\n';
   }
   for(Map_doc::const_iterator i = md.begin(); i != md.end(); ++i)
      std::cout << *i << '\n';
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_doc_search ) {
   Map_doc md;
   BOOST_CHECK( md.begin() == md.end());
   const Doc_id did1 = md.insert(nid1, path1, nid2).first;
   BOOST_CHECK_EQUAL( *md.begin(), did1 );
   BOOST_CHECK_EQUAL(md.find_iri(nid1).front(), did1);
   const Doc_id did2 = md.insert(nid2, path2, nid3).first;
   BOOST_CHECK_EQUAL(md.find_iri(nid2).front(), did2);
   BOOST_CHECK_EQUAL(distance(md.find_iri(nid1)), 1);
   BOOST_CHECK( ! md.find_iri(nid4) );

   BOOST_CHECK( ! md.find_version(nid1) );
   BOOST_CHECK_EQUAL(distance(md.find_version(nid2)), 1);
}

}//namespace test
}//namespace owlcpp
