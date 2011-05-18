/** @file "/owl_cpp/lib/test/find_ontologies_01_run.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010-1 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE find_ontologies_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>

#include "owl_cpp/parse_to_triple_store.hpp"
#include "sample_data.hpp"

namespace owl_cpp{ namespace test{

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
}//namespace owl_cpp