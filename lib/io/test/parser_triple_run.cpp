/** @file "/owlcpp/lib/io/test/parser_triple_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE parser_triple_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "triple_sink_printer.hpp"
#include "owlcpp/io/parser_triple.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = sample_file_path("imports_test_01.owl");

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Parser_triple parser;
   Triple_sink_printer tsp;
   parser(path1, tsp);

//   BOOST_ERROR("");
}

}//namespace test
}//namespace owlcpp
