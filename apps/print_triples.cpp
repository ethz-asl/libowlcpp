/** @file "/owlcpp/apps/print_triples.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/io/input.hpp"

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

/** Parse single OWL ontology file into triple store and print triples
*******************************************************************************/
int main(int argc, char* argv[]) {
   bpo::options_description od;
   od.add_options()
            ("help,h", "help message")
            ("input-file", bpo::value<std::string>(), "input OWL file")
            ("include,i",
                     bpo::value<std::vector<std::string> >()->zero_tokens()
                     ->composing(), "search paths")
            ("count,c", "count triples")
            ;
   bpo::positional_options_description pod;
   pod.add("input-file", -1);
   bpo::variables_map vm;
   store(bpo::command_line_parser(argc, argv).options(od).positional(pod).run(), vm);
   notify(vm);

   if( ! vm.count("input-file") || vm.count("help") ) {
      std::cout
      << "Usage:" << '\n'
      << "print_triples [-i[path]] [-c] <OWL_ontology_file.owl>" << '\n'
      << od << '\n';
      return 0;
   }

   owlcpp::Triple_store store;
   const bfs::path in(vm["input-file"].as<std::string>());
   try {
      if( vm.count("include") ) {
         owlcpp::Catalog cat;
         std::vector<std::string> const& vin = vm["include"].as<std::vector<std::string> >();
         if( vin.empty() ) {
            cat.add(in.parent_path());
         } else {
            BOOST_FOREACH(std::string const& p, vin) cat.add(p);
         }
         load_file(in, store, cat);
      } else {
         load_file(in, store);
      }
   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
