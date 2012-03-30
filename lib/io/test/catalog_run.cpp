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
   BOOST_CHECK_EQUAL(cat.size(), 0U);
   cat.insert_doc("some/path", Node_id(13), terms::T_empty_::id());
   BOOST_CHECK_EQUAL(cat.size(), 1U);
   cat.insert_doc(path1, ver1, version1);
   BOOST_CHECK_EQUAL(cat.size(), 2U);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Catalog cat;
   BOOST_CHECK_EQUAL(cat.size(), 0U);
   cat.add(path1);
   BOOST_CHECK_EQUAL(cat.size(), 1U);
   Doc_id const* did1 = cat.find_doc_iri(ver1);
   BOOST_REQUIRE(did1);
   BOOST_CHECK_EQUAL(cat.ontology_iri(*did1), ver1);
   BOOST_CHECK_EQUAL(cat.version_iri(*did1), version1);
   BOOST_CHECK_EQUAL(cat.path(*did1), boost::filesystem::canonical(path1).string());
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

   //blank node as ontologyIRI
   BOOST_CHECK_THROW(
            cat.insert_doc("path3", "_#Blah"),
            Catalog::Err
   );

   //empty namespace node as ontologyIRI
   BOOST_CHECK_THROW(
            cat.insert_doc("path3", "#Blah"),
            Catalog::Err
   );
}

/** Check sample data catalog
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Catalog cat;
   BOOST_CHECK_GE(cat.add(dir1), sample_files().size());
   BOOST_FOREACH(Sample_info const si, sample_files()) {
      Catalog::doc_map_t::path_range r = cat.documents().find_path(si.path);
      BOOST_REQUIRE(r);
      const Doc_id did = r.front();
      BOOST_CHECK_EQUAL(cat.ontology_iri(did), si.iri);
      BOOST_CHECK_EQUAL(cat.version_iri(did), si.version);
   }
}

}//namespace test
}//namespace owlcpp
