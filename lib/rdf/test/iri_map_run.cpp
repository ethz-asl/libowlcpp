/** @file "/owlcpp/lib/rdf/test/iri_map_run.cpp"
part of %owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#define BOOST_TEST_MODULE iri_map_run
#include "boost/test/unit_test.hpp"
#include <map>
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "type_vector.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string iri1("http://iri1");
const std::string iri2("http://iri2");
const std::string iri3("http://iri3");

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Iri_map im;
   const Ns_id id1 = im.insert(iri1);
   BOOST_CHECK_MESSAGE( id1 == im.insert(iri1), "same ID from inserting duplicates" );
   const Ns_id id2 = im.insert(iri2);
   BOOST_CHECK_NE(id1, id2);
   BOOST_CHECK_EQUAL(im[id1], iri1);
   BOOST_CHECK_EQUAL(im.at(id1), iri1);
   BOOST_CHECK_MESSAGE( im.find_prefix(id1) == "", "should return empty string" );
   im.remove(iri1);
   BOOST_CHECK_THROW(im.at(id1), Iri_map::Err);
   BOOST_CHECK_THROW(im.remove(iri1), Iri_map::Err);
   BOOST_CHECK_THROW(im.remove(id1), Iri_map::Err);
   const Ns_id id3 = im.insert(iri3);
   BOOST_CHECK_MESSAGE(id1 == id3, "id should be recycled");
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Iri_map im;
   const Ns_id id1 = im.insert(iri1);
   im.insert_prefix(id1, "iri1");
   BOOST_REQUIRE( im.find_iri(iri1) );
   BOOST_CHECK_EQUAL( id1, *im.find_iri(iri1) );
   BOOST_REQUIRE( im.find_prefix("iri1") );
   BOOST_CHECK_EQUAL( id1, *im.find_prefix("iri1") );
   BOOST_CHECK( ! im.find_iri(iri2) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Iri_map im;
   BOOST_CHECK_EQUAL( im[terms::N_owl::id()], terms::N_owl::iri() );
   BOOST_CHECK_EQUAL( im.find_prefix(terms::N_owl::id()), terms::N_owl::prefix() );
}

/** Copying IRIs from one IRI map to another
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Iri_map im1, im2;
   const Ns_id id11 = im1.insert(iri1);
   const Ns_id id23 = im2.insert(iri3);
   const Ns_id id21 = im2.insert(iri1);
   const Ns_id id22 = im2.insert(iri2);
   im2.insert_prefix(id22, "iri2");
   BOOST_CHECK_EQUAL(im1[id11], im2[id21]);
   BOOST_CHECK_THROW(im1.at(id22), Iri_map::Err);
   std::map<Ns_id,Ns_id> idmap;
   copy_iris(im2, im1, idmap);
   BOOST_CHECK_EQUAL(im1[id11], im2[id21]);
   BOOST_CHECK_EQUAL(idmap[id21], id11);
   BOOST_CHECK_EQUAL(im1[idmap[id23]], im2[id23]);
   BOOST_CHECK_EQUAL(im1[idmap[id22]], im2[id22]);
   BOOST_CHECK_EQUAL(im1.find_prefix(idmap[id22]), "iri2");
}

}//namespace test
}//namespace owlcpp
