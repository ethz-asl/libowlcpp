/** @file "/owlcpp/lib/test/triple_to_axiom_speed.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "boost/exception/diagnostic_information.hpp"
#include "boost/filesystem.hpp"
#include "boost/chrono/chrono.hpp"
#include "boost/chrono/chrono_io.hpp"
#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "owlcpp/logic/query_fact.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace bfs = boost::filesystem;
namespace ot = owlcpp::terms;

/**
Measure axiom generation rate
*******************************************************************************/
int main(int argc, char* argv[]) {
   if( argc < 2 ) {
      std::cout << "Usage: triple_to_axiom path1" << std::endl;
      return 0;
   }

   try{
      owlcpp::Catalog cat;
      const bfs::path in = argv[1];
      add(cat, in.parent_path(), true);
      owlcpp::Triple_store ts;
      typedef boost::chrono::high_resolution_clock clock_t;
      typedef clock_t::time_point time_t;
      const time_t t0 = clock_t::now();
      load_file(in, ts, cat);
      typedef boost::chrono::duration<double, boost::nano> dur_t;
      const dur_t d0 = clock_t::now() - t0;
        std::cout
        << d0 / ts.map_triple().size() << " per triple" << '\n'
        << ts.map_triple().size() << " triples" << '\n'
        << ts.map_node().size() << " nodes" << '\n'
        << ts.map_ns().size() << " namespace IRIs" << '\n'
        ;

        ReasoningKernel k;
      const time_t t1 = clock_t::now();
        submit_triples(ts, k);
      const dur_t d1 = clock_t::now() - t1;
        std::cout
        << d1 / ts.map_triple().size() << " per triple" << '\n'
        ;
   }catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
