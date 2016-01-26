/** @file "/owlcpp/lib/rdf/test/ns_iri_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE ns_iri_run
#include "boost/test/unit_test.hpp"

#include "owlcpp/rdf/ns_iri.hpp"
#include "test/exception_fixture.hpp"

namespace owlcpp{ namespace test{

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   const std::string str1 = "http://example.com";
   const std::string str2 = str1 + "#frag";
   const Ns_iri ns1(str1);
   const Ns_iri ns2 = remove_fragment(str1);
   BOOST_CHECK_EQUAL(ns1, ns2);
   const Ns_iri ns3 = remove_fragment(str2);
   BOOST_CHECK_EQUAL(ns1, ns3);

   std::size_t n;
   remove_fragment(str1, n);
   BOOST_CHECK_EQUAL(n, str1.size());
   remove_fragment(str2, n);
   BOOST_CHECK_EQUAL(str2.substr(n), "frag");
}

}//namespace test
}//namespace owlcpp
