/** @file "/owlcpp/lib/rdf/test/map_ns_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_ns_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_ns.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_ns_01 ) {
   Map_ns mns1(Ns_id(5));
   BOOST_CHECK_EQUAL(mns1.size(), 0u);
   const Ns_id id1 = mns1.insert("i1");
   BOOST_CHECK(mns1.find("i1"));
   BOOST_CHECK_EQUAL(*mns1.find("i1"), id1);
   const Ns_id id2 = mns1.insert("i2");
   mns1.set_prefix(id2, "p2");
   BOOST_CHECK_EQUAL(mns1.prefix(id2), "p2");
   BOOST_CHECK(mns1.find_prefix("p2"));

   const Ns_id id10 = mns1.insert("i10");
   BOOST_CHECK_EQUAL(mns1.size(), 3U);
   BOOST_CHECK(mns1.prefix(id10).empty());
   mns1.set_prefix(id10, "p10");
   BOOST_CHECK_EQUAL(mns1.prefix(id10), "p10");
   mns1.set_prefix(id10);
   BOOST_CHECK_EQUAL(mns1.prefix(id10), "");
   BOOST_CHECK_EQUAL(mns1.insert("i10"), id10);

   Map_ns mns2(mns1);

   mns1.remove(id2);
   BOOST_CHECK_EQUAL(mns1.size(), 2U);
   BOOST_CHECK( ! mns1.find_prefix("p2"));
   BOOST_CHECK( ! mns1.find(id2) );

   BOOST_CHECK(  mns2.find_prefix("p2"));
   BOOST_CHECK(  mns2.find(id2) );
   BOOST_CHECK_EQUAL(mns2[id2], "i2");
   BOOST_CHECK_EQUAL(mns2.prefix(id2), "p2");

   mns1.remove(id10);
   BOOST_CHECK( ! mns1.find(id10) );
   mns1.remove(id1);
   BOOST_CHECK( ! mns1.find(id1) );

   const Ns_id id13 = mns1.insert("i13");
   //removed IDs are recycled
   BOOST_CHECK( id13 == id1 || id13 == id2 || id13 == id10 );


}

}//namespace test
}//namespace owlcpp
