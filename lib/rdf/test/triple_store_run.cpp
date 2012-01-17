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

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;
}

}//namespace test
}//namespace owlcpp
