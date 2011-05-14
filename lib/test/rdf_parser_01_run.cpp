/** @file "/owl_cpp/lib/test/rdf_parser_01_run.cpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010-1 @author Mikhail K Levin
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE rdf_parser_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "sample_data.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/query_triples.hpp"
#include "triple_store_checks.hpp"

namespace owl_cpp{

namespace test{

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( rdf_parser_01_union ) {
	const char* f_name = "union_01.owl";
	std::cout << '\n' << f_name << std::endl;
	const std::string f_path = sample_file_path(f_name);
   Triple_store store;
   load(f_path, store);
   BOOST_CHECK( check_and_print(store, std::cout) );
//   print(store, std::cout);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( rdf_parser_01_trans_property ) {
	const char* f_name = "transitive_property.owl";
	std::cout << '\n' << f_name << std::endl;
	const std::string f_path = sample_file_path(f_name);
   Triple_store store;
   try{
      load(f_path, store);
   } catch(...) {
      std::cerr << b::current_exception_diagnostic_information() << std::endl;
      throw;
   }

   BOOST_CHECK( check_and_print(store, std::cout) );

//   print(store, std::cout);
}

}//namespace test
}//namespace owl_cpp
