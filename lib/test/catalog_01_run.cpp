/** @file "/owlcpp/lib/test/catalog_01_run.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE catalog_01_run
#include "boost/test/unit_test.hpp"
#include "owlcpp/io/catalog.hpp"

namespace owlcpp{ namespace test{

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
}//namespace owlcpp
