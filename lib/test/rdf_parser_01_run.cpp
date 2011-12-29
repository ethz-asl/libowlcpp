/** @file "/owlcpp/lib/test/rdf_parser_01_run.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE rdf_parser_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/print.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "triple_store_checks.hpp"
#include "fixture.hpp"

namespace owlcpp{

namespace test{
BOOST_FIXTURE_TEST_SUITE(suite, Fixture);

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( rdf_parser_01_union ) {
   const char* f_name = "union_01.owl";
//   std::cout << '\n' << f_name << std::endl;
   const std::string f_path = sample_data_path(f_name);
   Triple_store store;
   BOOST_CHECK_EQUAL( store.n_nodes(), 91U );
   BOOST_CHECK_EQUAL( store.n_triples(), 0U );
   load(f_path, store);
//   print(store, std::cout);
   BOOST_CHECK_EQUAL( store.n_nodes(), 98U );
   BOOST_CHECK_EQUAL( store.n_triples(), 12U );
   check(store);
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
}//namespace owlcpp
