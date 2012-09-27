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
#include "owlcpp/rdf/store_triple.hpp"
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
            ("count,c", "only print the number of triples")
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
   const bfs::path in( vm["input-file"].as<std::string>());
   try {
      if( vm.count("include") ) { //load input-file and its includes
         owlcpp::Catalog cat;
         //how far to look into each file for ontologyVersion
         const std::size_t search_depth = 100;
         std::vector<std::string> const& vin = vm["include"].as<std::vector<std::string> >();
         if( vin.empty() ) {
            cat.add(in.parent_path(), true, search_depth);
         } else {
            BOOST_FOREACH(std::string const& p, vin) cat.add(p, true, search_depth);
         }
         load_file(in, store, cat);
      } else { //load just input-file
         load_file(in, store);
      }
      if( vm.count("count") ) {
         std::cout
         << store.triples().size() << " triples" << '\n'
         << store.nodes().size() << " nodes" << '\n'
         << distance(store.nodes().find(owlcpp::terms::N_empty::id())) << " literal nodes" << '\n'
         << distance(store.nodes().find(owlcpp::terms::N_blank::id())) << " blank nodes" << '\n'
         << store.iris().size() << " namespace IRIs" << '\n'
         ;
      } else {
         BOOST_FOREACH( owlcpp::Triple const& t, store.triples() ) {
            std::cout
            << '\"'
            << store.string(t.subject()) << "\"\t\""
            << store.string(t.predicate()) << "\"\t\""
            << store.string(t.object()) << "\"\t\n"
            ;
         }
      }

   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
