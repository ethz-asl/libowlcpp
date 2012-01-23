/** @file "/owlcpp/lib/io/test/ontology_id_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE ontology_id_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/io/catalog.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = "version_test_b.owl";
const std::string iri1 = "http://purl.obolibrary.org/obo/ido/dev/version_test.owl";
const std::string version1 = "http://purl.obolibrary.org/obo/ido/dev/version_test_b.owl";
const std::string path2 = "imports_test_01.owl";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   std::pair<std::string,std::string> p1 = ontology_id(sample_file_path(path1));
   BOOST_CHECK_EQUAL(p1.first, iri1);
   BOOST_CHECK_EQUAL(p1.second, version1);
}

}//namespace test
}//namespace owlcpp
