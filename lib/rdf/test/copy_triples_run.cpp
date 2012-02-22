/** @file "/owlcpp/lib/rdf/test/copy_triples_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE copy_triples_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_triples.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/copy_triples.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   const Triple_store ts1 = sample_triples_01();
   Triple_store ts2 = sample_triples_02();
   copy_triples(ts1, ts2);

   BOOST_CHECK( ts2.documents().find_path(path1) );
   BOOST_CHECK( ts2.documents().find_path(path2) );
}

}//namespace test
}//namespace owlcpp
