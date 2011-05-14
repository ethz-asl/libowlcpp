/** @file "/owl_cpp/lib/test/catalog_01_run.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE catalog_01_run
#include "boost/test/unit_test.hpp"
#include "owl_cpp/catalog.hpp"

namespace owl_cpp{ namespace test{

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( catalog_01_run_case01 ) {
   Catalog cat;

   //empty location
   BOOST_CHECK_THROW( cat.insert("", "iri1", ""), Catalog::Err );

   //empty IRI
   BOOST_CHECK_THROW( cat.insert("path1", "", "iri1"), Catalog::Err );

   //empty IRI and version
   BOOST_CHECK_THROW( cat.insert("path1", "", ""), Catalog::Err );

   BOOST_CHECK_NO_THROW( cat.insert("path1", "iri1", "") );

   //same location, same ontology
   BOOST_CHECK_NO_THROW( cat.insert("path1", "iri1", "") );

   //same location, different ontology
   BOOST_CHECK_THROW( cat.insert("path1", "iri2", ""), Catalog::Err );

   //different location, same ontology
   BOOST_CHECK_THROW( cat.insert("path2", "iri1", ""), Catalog::Err );

   //different location, different version
   BOOST_CHECK_NO_THROW( cat.insert("path2", "iri1", "iri1") );

   //different location, same version
   BOOST_CHECK_THROW( cat.insert("path3", "iri2", "iri1"), Catalog::Err );

   //search first by version
   BOOST_CHECK_EQUAL(cat.find_location("iri1"), "path2");
}

}//namespace test
}//namespace owl_cpp
