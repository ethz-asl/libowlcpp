/** @file "/owlcpp/lib/test/time_axioms.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012-5
*******************************************************************************/
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "boost/exception/diagnostic_information.hpp"
#include "boost/chrono/chrono.hpp"
#include "boost/chrono/chrono_io.hpp"
#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
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
      const bfs::path in = argv[1];
      typedef boost::chrono::high_resolution_clock clock_t;
      typedef clock_t::time_point time_t;
      typedef boost::chrono::duration<double, boost::ratio<1,1> > dur_t;

      const time_t t0 = clock_t::now();
      owlcpp::Catalog cat;
      add(cat, in.parent_path(), false, 100);
      const dur_t d0 = clock_t::now() - t0;
      std::cout
      << "cataloging:\n"
      << d0 << '\n'
      ;

      owlcpp::Triple_store ts;
      const time_t t1 = clock_t::now();
      load_file(in, ts, cat);
      const dur_t d1 = clock_t::now() - t1;
      std::cout
      << "parsing:\n"
      << d1 << '\n'
      << ts.map_triple().size() << " triples" << '\n'
      << ts.map_node().size() << " nodes" << '\n'
      << ts.map_ns().size() << " namespace IRIs" << '\n'
      ;

      ReasoningKernel k;
      const time_t t2 = clock_t::now();
      const std::size_t n = submit(ts, k);
      const dur_t d2 = clock_t::now() - t2;
      std::cout
      << "axiom generation:\n"
      << n << " axioms" << '\n'
      << d2 << '\n'
      ;
   }catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
