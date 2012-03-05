/** @file "/owlcpp/lib/test/print_01_run.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE print_01_run
#include "boost/test/unit_test.hpp"

#include <iostream>
#include "sample_data.hpp"
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/print.hpp"
#include "owlcpp/lib_info.hpp"
#include "owlcpp/terms/term_tags.hpp"
namespace ot = owlcpp::terms;

namespace owlcpp{ namespace test{
/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( print_01_run_case01 ) {
   const char* f_name = "union_01.owl";
   std::cout << '\n' << f_name << std::endl;
   const std::string f_path = sample_file_path(f_name);
   Triple_store store;
   load(f_path, store);
//   print(store, std::cout);
   std::cout << std::endl;
   const Node_id nid1 = insert_node("http://www.ifomis.org/bfo/1.1", store);
//   expand(nid1, store, std::cout);
   BOOST_CHECK_EQUAL(expand(nid1, store), "http://www.ifomis.org/bfo/1.1");
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( print_01_run_case02 ) {
   Triple_store store;
   BOOST_CHECK_EQUAL(
         expand(ot::T_owl_bottomObjectProperty::id(), store),
         "http://www.w3.org/2002/07/owl#bottomObjectProperty"
   );

   BOOST_CHECK_EQUAL(
         full_name(ot::T_owl_bottomObjectProperty()),
         "http://www.w3.org/2002/07/owl#bottomObjectProperty"
   );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( print_01_run_case03 ) {
   std::cout << std::endl;
   Lib_info::print_version(std::cout << "version: ")
   << " build:" << Lib_info::build() << std::endl;
}

}//namespace test
}//namespace owlcpp
