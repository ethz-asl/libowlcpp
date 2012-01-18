/** @file "/owlcpp/lib/rdf/test/triple_store_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

const std::string ni1 = "http://example.xyz/example#n1";
const std::string ni2 = "http://example.xyz/example#n2";
/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;

   const Node_id nid1 = ts.insert_iri_node(ni1);
   const Node_id nid1a = ts.insert_iri_node(ni1);
   BOOST_CHECK_EQUAL(nid1, nid1a);
   const Node_id nid2 = ts.insert_iri_node(ni2);
   BOOST_CHECK_NE(nid1, nid2);

   //same namespace IRIs
   BOOST_CHECK_EQUAL(ts[nid1].ns_id(), ts[nid2].ns_id());
   //different fragment names
   BOOST_CHECK_NE(ts[nid1].value_str(), ts[nid2].value_str());
}

}//namespace test
}//namespace owlcpp
