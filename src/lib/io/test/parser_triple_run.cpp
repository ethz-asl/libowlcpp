/** @file "/owlcpp/lib/io/test/parser_triple_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE parser_triple_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "boost/foreach.hpp"
#include "boost/filesystem/fstream.hpp"
#include "test/sample_data.hpp"
#include "triple_printer.hpp"
#include "owlcpp/io/raptor_wrapper.hpp"

namespace owlcpp{ namespace test{

/**@test print triples from sample ontologies
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_parser ) {
   BOOST_FOREACH(Sample_info const& si, sample_files()) {
      std::cout << si.path << '\n';
      Raptor_wrapper parser;
      Triple_printer tsp;
      boost::filesystem::ifstream ifs(si.path);
      parser(ifs, tsp);
      std::cout << '\n';
   }
   //BOOST_ERROR("");
}

}//namespace test
}//namespace owlcpp
