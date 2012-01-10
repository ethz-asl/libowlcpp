/** @file "/owlcpp/lib/rdf/test/iri_store_owl_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE iri_store_owl_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/iri_store_owl.hpp"
#include "owlcpp/terms/iri_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Iri_store_owl is;
   BOOST_CHECK_EQUAL( is[terms::N_owl::id()], terms::N_owl::iri() );
   BOOST_CHECK_EQUAL( *is.find_prefix(terms::N_owl::id()), terms::N_owl::prefix() );
}

}//namespace test
}//namespace owlcpp
