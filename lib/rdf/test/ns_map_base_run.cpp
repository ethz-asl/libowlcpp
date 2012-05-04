/** @file "/owlcpp/lib/rdf/test/ns_map_base_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE ns_map_base_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/ns_map_base.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Ns_map_base map;
   BOOST_CHECK_EQUAL(map.size(), 0u);
   const Ns_id id1 = map.insert("i1");
   BOOST_CHECK(map.find_iri("i1"));
   BOOST_CHECK_EQUAL(*map.find_iri("i1"), id1);
   const Ns_id id2 = map.insert("i2");
   map.set_prefix(id2, "p2");
   BOOST_CHECK_EQUAL(map.prefix(id2), "p2");
   BOOST_CHECK(map.find_prefix("p2"));

   const Ns_id id10 = map.insert(Ns_id(10), "i10");
   BOOST_CHECK_EQUAL(map.size(), 3U);
   BOOST_CHECK(map.prefix(id10).empty());
   map.set_prefix(id10, "p10");
   BOOST_CHECK_EQUAL(map.prefix(id10), "p10");
   map.set_prefix(id10);
   BOOST_CHECK_EQUAL(map.prefix(id10), "");
   BOOST_CHECK_EQUAL(map.insert("i10"), id10);

   map.remove(id2);
   BOOST_CHECK_EQUAL(map.size(), 2U);
   BOOST_CHECK(! map.find_prefix("p2"));
   BOOST_CHECK( ! map.have(id2) );

   map.remove(id10);
   BOOST_CHECK( ! map.have(id10) );
   map.remove(id1);
   BOOST_CHECK( ! map.have(id1) );

   const Ns_id id13 = map.insert("i13");
   //removed IDs are recycled
   BOOST_CHECK( id13 == id1 || id13 == id2 || id13 == id10 );
}

}//namespace test
}//namespace owlcpp
