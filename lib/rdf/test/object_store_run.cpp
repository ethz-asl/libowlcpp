/** @file "/owlcpp/lib/rdf/test/object_store_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE object_store_run
#include "boost/test/unit_test.hpp"
#include <string>
#include "test/exception_fixture.hpp"
#include "owlcpp/detail/object_store.hpp"
#include "owlcpp/ns_id.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef owlcpp::detail::Object_store<std::string> store_t;
   store_t st;
   BOOST_CHECK_EQUAL(st.size(), 0U);
   BOOST_CHECK( ! st.have(0) );
   const std::string s1 = "blah";
   const unsigned id1 = st.insert(s1);
   BOOST_CHECK( st.have(id1) );
   BOOST_CHECK(st.find(s1)->second == id1);

   const std::string s2 = "boo";
   const unsigned id2 = 10;
   BOOST_CHECK_THROW(st.at(id2), store_t::Err);
   st.insert(id2, s2);
   BOOST_CHECK_EQUAL(st.at(id2)->first, s2);
   BOOST_CHECK_EQUAL(st.insert(s2), id2);
   BOOST_CHECK(st[id2]->first == s2);
   BOOST_CHECK(st[8] == st.end());
}

}//namespace test
}//namespace owlcpp
