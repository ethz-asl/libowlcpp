/** @file "/owlcpp/lib/io/test/catalog_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE catalog_run
#include "boost/test/unit_test.hpp"
#include "boost/filesystem.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = sample_file_path("version_test_b.owl");
const std::string ver1 = "http://purl.obolibrary.org/obo/ido/dev/version_test.owl";
const std::string version1 = "http://purl.obolibrary.org/obo/ido/dev/version_test_b.owl";
const std::string dir1 = sample_file_path();

/** Insert doc info manually
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Catalog cat;
   const Node_id nid1 = cat.insert_node_iri("http://some.node.com");
   const Node_id nid2 = cat.insert_node_iri("http://other.node.com");
   BOOST_CHECK_EQUAL(cat.size(), 0U);
   const Doc_id did1 = cat.insert_doc(nid1, "some/path", terms::T_empty_::id()).first;
   const Doc_id did2 = cat.insert_doc(nid2, "other/path").first;
   const Doc_id did2a = cat.insert_doc(nid2, "other/path", terms::T_empty_::id()).first;
   BOOST_CHECK_EQUAL(cat.size(), 2U);
   BOOST_CHECK_EQUAL(did2, did2a);
   BOOST_CHECK_EQUAL(cat[did1].ontology_iri, nid1);

   cat.insert_doc(path1, ver1, version1);
   BOOST_CHECK_EQUAL(cat.size(), 3U);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Catalog cat;
   BOOST_CHECK_EQUAL(cat.size(), 0U);
   add(cat, path1);
   BOOST_CHECK_EQUAL(cat.size(), 1U);
   Catalog::doc_iri_range r = cat.find_doc_iri(ver1);
   BOOST_REQUIRE(r);
   BOOST_CHECK_EQUAL(cat.ontology_iri_str(*r.begin()), ver1);
   BOOST_CHECK_EQUAL(cat.version_iri_str(*r.begin()), version1);
   BOOST_CHECK_EQUAL(boost::filesystem::canonical(path1).string(), cat[*r.begin()].path);
}

/** Test inserting new nodes into OWL namespace
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Catalog cat;

   //standard term as document IRI
   BOOST_CHECK_NO_THROW(
            cat.insert_doc("path1", terms::N_owl::iri() + "#Ontology")
   );

   //wrong term in standard namespace as ontologyIRI
   BOOST_CHECK_NO_THROW(
            cat.insert_doc("path2", terms::N_owl::iri() + "#Blah")
   );
}

/** Check sample data catalog
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_catalog_sample_data ) {
   Catalog cat;
   BOOST_CHECK_EQUAL(cat.size(), 0U);
   const std::size_t n = add(cat, dir1);
   BOOST_CHECK_EQUAL(n, cat.size());
   BOOST_CHECK_GE(cat.size(), sample_files().size());

   BOOST_FOREACH(Sample_info const si, sample_files()) {
      Catalog::map_doc_type::path_range r = cat.map_doc().find_path(si.path);
      BOOST_REQUIRE(r);
      const Doc_id did = r.front();
      BOOST_REQUIRE(cat.valid(did));
      BOOST_CHECK_EQUAL(cat.path(did), si.path);
      const Node_id iri_id = cat.ontology_iri_id(did);
      BOOST_REQUIRE(cat.valid(iri_id));
      const Node_id vers_id = cat.version_iri_id(did);
      BOOST_REQUIRE(cat.valid(vers_id));
      BOOST_CHECK_EQUAL(cat.ontology_iri_str(did), si.iri);
      BOOST_CHECK_EQUAL(cat.version_iri_str(did), si.version);
   }
}

}//namespace test
}//namespace owlcpp
