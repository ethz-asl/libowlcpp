/** @file "/owlcpp/lib/test/find_ontologies_01_run.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#define BOOST_TEST_MODULE find_ontologies_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>

#include "owlcpp/io/parse_to_triple_store.hpp"
#include "sample_data.hpp"

namespace owlcpp{ namespace test{

/** Read ontology and version IRIs
*******************************************************************************/
BOOST_AUTO_TEST_CASE( get_uri_01_run_case01 ) {

   BOOST_CHECK_EQUAL(
         ontology_id(sample_file_path("version_test_b.owl")).get<0>(),
         "http://purl.obolibrary.org/obo/ido/dev/version_test.owl"
   );

   BOOST_CHECK_EQUAL(
         ontology_id(sample_file_path("version_test_b.owl")).get<1>(),
         "http://purl.obolibrary.org/obo/ido/dev/version_test_b.owl"
   );
}

/** Populate catalog
*******************************************************************************/
BOOST_AUTO_TEST_CASE( make_catalog_case01 ) {
   Catalog cat;
   try{
      find_ontologies(cat, sample_file_path(""));
   } catch(b::exception& e) {
      std::cout << b::diagnostic_information(e) << std::endl;
      throw;
   }
   BOOST_CHECK_NO_THROW( cat.find_location("http://owl2.test/rules/") );
}


}//namespace test
}//namespace owlcpp
