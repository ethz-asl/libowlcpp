/** @file "/owlcpp/lib/rdf/test/triple_query_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_query_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"

#include "owlcpp/rdf/triple_query.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_map tm;
   tm.insert(Triple(Node_id(0), Node_id(0), Node_id(0), Doc_id(0)));
   tm.find(blank(), blank(), blank(), blank());
//   tm.find(Node_id(0), blank(), blank(), blank());

}

}//namespace test
}//namespace owlcpp
