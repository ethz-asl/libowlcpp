/** @file "/owl_cpp/lib/test/rdf_parser_02_run.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#define BOOST_TEST_MODULE rdf_parser_02_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "sample_data.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
//#include "owl_cpp/triple_store.hpp"
//#include "owl_cpp/print.hpp"
//#include "owl_cpp/query_triples.hpp"
//#include "triple_store_checks.hpp"

namespace owl_cpp{ namespace test{

/** Test safety
*******************************************************************************/
BOOST_AUTO_TEST_CASE( rdf_parser_02_run_case01 ) {
   BOOST_CHECK_NO_THROW( ontology_id(sample_file_path("test_01.owl")) );

   BOOST_CHECK_THROW( ontology_id(sample_file_path("wrong_name.owl")), b::exception );
}

}//namespace test
}//namespace owl_cpp
