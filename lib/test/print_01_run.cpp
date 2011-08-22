/** @file "/owl_cpp/lib/test/print_01_run.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE print_01_run
#include "boost/test/unit_test.hpp"

#include <iostream>
#include "sample_data.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/lib_info.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp{ namespace test{
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
   std::cout
   << "version:" << version_str() << ' '
   << "code revision:" << version_str() << ' '
   << "build:" << build_str() << '\n';
   std::cout << lib_info_str();
}

}//namespace test
}//namespace owl_cpp
