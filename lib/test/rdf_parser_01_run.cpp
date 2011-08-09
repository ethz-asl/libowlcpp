/** @file "/owl_cpp/lib/test/rdf_parser_01_run.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE rdf_parser_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/query_triples.hpp"
#include "triple_store_checks.hpp"
#include "fixture.hpp"

namespace owl_cpp{

namespace test{
BOOST_FIXTURE_TEST_SUITE(suite, Fixture);

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( rdf_parser_01_union ) {
   const char* f_name = "union_01.owl";
//   std::cout << '\n' << f_name << std::endl;
   const std::string f_path = sample_data_path(f_name);
   Triple_store store;
   load(f_path, store);
   check(store);
//   print(store, std::cout);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( rdf_parser_01_trans_property ) {
   const char* f_name = "transitive_property.owl";
//   std::cout << '\n' << f_name << std::endl;
   const std::string f_path = sample_data_path(f_name);
   Triple_store store;
   load(f_path, store);

   check(store);
//   print(store, std::cout);
}

BOOST_AUTO_TEST_SUITE_END();
}//namespace test
}//namespace owl_cpp
