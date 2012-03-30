/** @file "/owlcpp/lib/io/test/triple_store_print_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_print_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   BOOST_FOREACH(Sample_info const& si, sample_files()) {
      std::cout << si.path << '\n';
      Triple_store ts(Node_map_std::get(Nodes_owl()));
      load_file(si.path, ts);
      BOOST_FOREACH(Triple const& t, ts.triples()) {
         std::cout
         << to_string_shortest(t.subject(), ts) << ' '
         << to_string_shortest(t.predicate(), ts) << ' '
         << to_string_shortest(t.object(), ts) << '\n'
         ;
      }
      std::cout << '\n';
   }
}

}//namespace test
}//namespace owlcpp
