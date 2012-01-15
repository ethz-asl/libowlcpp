/** @file "/owlcpp/lib/rdf/test/iri_store01_run.cpp"
part of %owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#define BOOST_TEST_MODULE iri_store01
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/iri_store.hpp"
#include "type_vector.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

const std::string iri1("http://iri1");
const std::string iri2("http://iri2");
const std::string iri3("http://iri3");

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( iri_store01_case01 ) {
   Iri_store is;
   const Ns_id id1 = is.insert(iri1);
   BOOST_CHECK_MESSAGE( id1 == is.insert(iri1), "same ID from inserting duplicates" );
   const Ns_id id2 = is.insert(iri2);
   BOOST_CHECK_EQUAL(is[id1], iri1);
   BOOST_CHECK_EQUAL(is.at(id1), iri1);
   BOOST_CHECK_MESSAGE( is.find_prefix(id1) == "", "should return empty string" );
   is.remove(iri1);
   BOOST_CHECK_THROW(is.at(id1), Iri_store::Err);
   BOOST_CHECK_THROW(is.remove(iri1), Iri_store::Err);
   BOOST_CHECK_THROW(is.remove(id1), Iri_store::Err);
   const Ns_id id3 = is.insert(iri3);
   BOOST_CHECK_MESSAGE(id1 == id3, "id should be recycled");
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( iri_store01_case02 ) {
   Iri_store is;
   const Ns_id id1 = is.insert(iri1, "iri1");
   BOOST_REQUIRE( is.find_iri(iri1) );
   BOOST_CHECK_EQUAL( id1, *is.find_iri(iri1) );
   BOOST_REQUIRE( is.find_prefix("iri1") );
   BOOST_CHECK_EQUAL( id1, *is.find_prefix("iri1") );
   BOOST_CHECK( ! is.find_iri(iri2) );
}

}//namespace test
}//namespace owlcpp
