/** @file "/owlcpp/lib/rdf/test/map_ns_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_ns_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_ns.hpp"
#include "owlcpp/rdf/nodes_std.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace t = owlcpp::terms;

const std::string iri1("http://iri1");
const std::string iri2("http://iri2");
const std::string iri3("http://iri3");

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_ns mns(( Nodes_none() ));
   const Ns_id id1 = mns.insert(iri1);
   BOOST_CHECK_MESSAGE( id1 == mns.insert(iri1), "same ID from inserting duplicates" );
   const Ns_id id2 = mns.insert(iri2);
   BOOST_CHECK_NE(id1, id2);
   BOOST_CHECK_EQUAL(mns[id1], iri1);
   BOOST_CHECK_EQUAL(mns.at(id1), iri1);
   BOOST_CHECK_MESSAGE( mns.prefix(id1) == "", "should return empty string" );
   mns.remove(id1);
   BOOST_CHECK_THROW(mns.at(id1), Map_ns::Err);
   BOOST_CHECK_THROW(mns.remove(id1), Map_ns::Err);
   const Ns_id id3 = mns.insert(iri3);
   BOOST_CHECK_MESSAGE(id1 == id3, "id should be recycled");
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Map_ns mns(( Nodes_none() ));
   const Ns_id id1 = mns.insert(iri1);
   mns.insert_prefix(id1, "iri1");
   BOOST_REQUIRE( mns.find_iri(iri1) );
   BOOST_CHECK_EQUAL( id1, *mns.find_iri(iri1) );
   BOOST_REQUIRE( mns.find_prefix("iri1") );
   BOOST_CHECK_EQUAL( id1, *mns.find_prefix("iri1") );
   BOOST_CHECK( ! mns.find_iri(iri2) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Map_ns mns(( Nodes_owl() ));
   BOOST_CHECK_EQUAL( mns[t::N_owl::id()], t::N_owl::iri() );
   BOOST_CHECK_EQUAL( mns.prefix(t::N_owl::id()), t::N_owl::prefix() );
}

/** Check prefix insertion
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Map_ns mns(( Nodes_owl() ));
   const Ns_id id1 = mns.insert(iri1);
   const Ns_id id2 = mns.insert(iri2);
   const Ns_id id3 = mns.insert(iri3);
   BOOST_CHECK( mns.prefix(id1) == "" );
   BOOST_CHECK( mns.prefix(id2) == "" );
   BOOST_CHECK( mns.prefix(id3) == "" );
   mns.insert_prefix(id1, "iri1");
   BOOST_CHECK( mns.prefix(id1) == "iri1" );
   mns.insert_prefix(id2, "iri2");
   BOOST_CHECK( mns.prefix(id2) == "iri2" );
   mns.insert_prefix(id3, "iri3");
   BOOST_CHECK( mns.prefix(id3) == "iri3" );
}

}//namespace test
}//namespace owlcpp
