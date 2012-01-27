/** @file "/owlcpp/lib/io/test/ontology_id_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE ontology_id_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/io/read_ontology_iri.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = sample_file_path("version_test_b.owl");
const std::string iri1 = "http://purl.obolibrary.org/obo/ido/dev/version_test.owl";
const std::string version1 = "http://purl.obolibrary.org/obo/ido/dev/version_test_b.owl";

const std::string path2 = sample_file_path("imports_test_01.owl");
const std::string iri2 = "http://purl.obolibrary.org/obo/ido/dev/imports_test_01.owl";

const std::string path3 = sample_file_path("negative_property_assertion_01.owl");
const std::string iri3 = "http://owl.semanticweb.org/page/New-Feature-NegativeObjectPropertyAssertion-001";

const std::string path4 = sample_file_path("one_eq_two_01.owl");
const std::string iri4 = "http://example.com/one_eq_two_01.owl";

const std::string path5 = sample_file_path("owl2-rl-rules-fp-differentFrom.owl");
const std::string iri5 = "http://owl.semanticweb.org/page/Owl2-rl-rules-fp-differentFrom";

const std::string path6 = sample_file_path("owl2-rl-rules-fp-sameAs.owl");
const std::string iri6 = "http://owl.semanticweb.org/page/Owl2-rl-rules-fp-sameAs";

const std::string path7 = sample_file_path("owl2-rl-rules-ifp-differentFrom.owl");
const std::string iri7 = "http://owl.semanticweb.org/page/Owl2-rl-rules-ifp-differentFrom";

const std::string path8 = sample_file_path("owl2-rl-rules-ifp-sameAs.owl");
const std::string iri8 = "http://owl2.test/rules/owl2-rl-rules-ifp-sameAs";

const std::string path9 = sample_file_path("propertyChain_01.owl");
const std::string iri9 = "http://owl.semanticweb.org/page/New-Feature-ObjectPropertyChain-001";

const std::string path10 = sample_file_path("test_01.owl");
const std::string iri10 = "http://purl.obolibrary.org/obo/ido/dev/test_01.owl";
const std::string version10 = "http://purl.obolibrary.org/obo/ido/dev/test_01a.owl";

const std::string path11 = sample_file_path("transitive_property.owl");
const std::string iri11 = "http://purl.org/obo/owl/transitive_property";

const std::string path12 = sample_file_path("union_01.owl");
const std::string iri12 = "http://www.ifomis.org/bfo/union";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   const std::pair<std::string,std::string> p1 = read_ontology_iri(path1);
   BOOST_CHECK_EQUAL(p1.first, iri1);
   BOOST_CHECK_EQUAL(p1.second, version1);

   const std::pair<std::string,std::string> p2 = read_ontology_iri(path2);
   BOOST_CHECK_EQUAL(p2.first, iri2);
   BOOST_CHECK(p2.second.empty());

   const std::pair<std::string,std::string> p3 = read_ontology_iri(path3);
   BOOST_CHECK_EQUAL(p3.first, iri3);
   BOOST_CHECK(p3.second.empty());

   const std::pair<std::string,std::string> p4 = read_ontology_iri(path4);
   BOOST_CHECK_EQUAL(p4.first, iri4);
   BOOST_CHECK(p4.second.empty());

   const std::pair<std::string,std::string> p5 = read_ontology_iri(path5);
   BOOST_CHECK_EQUAL(p5.first, iri5);
   BOOST_CHECK(p5.second.empty());

   const std::pair<std::string,std::string> p6 = read_ontology_iri(path6);
   BOOST_CHECK_EQUAL(p6.first, iri6);
   BOOST_CHECK(p6.second.empty());

   const std::pair<std::string,std::string> p7 = read_ontology_iri(path7);
   BOOST_CHECK_EQUAL(p7.first, iri7);
   BOOST_CHECK(p7.second.empty());

   const std::pair<std::string,std::string> p8 = read_ontology_iri(path8);
   BOOST_CHECK_EQUAL(p8.first, iri8);
   BOOST_CHECK(p8.second.empty());

   const std::pair<std::string,std::string> p9 = read_ontology_iri(path9);
   BOOST_CHECK_EQUAL(p9.first, iri9);
   BOOST_CHECK(p9.second.empty());

   const std::pair<std::string,std::string> p10 = read_ontology_iri(path10);
   BOOST_CHECK_EQUAL(p10.first, iri10);
   BOOST_CHECK_EQUAL(p10.second, version10);

   const std::pair<std::string,std::string> p11 = read_ontology_iri(path11);
   BOOST_CHECK_EQUAL(p11.first, iri11);
   BOOST_CHECK(p11.second.empty());

   const std::pair<std::string,std::string> p12 = read_ontology_iri(path12);
   BOOST_CHECK_EQUAL(p12.first, iri12);
   BOOST_CHECK(p12.second.empty());
}

}//namespace test
}//namespace owlcpp
