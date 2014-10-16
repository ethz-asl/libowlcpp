/** @file "/owlcpp/apps/print_classes.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "boost/range.hpp"
#include "boost/foreach.hpp"
#include "boost/filesystem.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

/**
Parse OWL ontology file and its imports located in the same folder
Find triples that define classes
Print prefixed names of classes
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
      << "Print triples that define classes" << '\n'
      << "Usage:" << '\n'
      << "print_classes [-i[path]] [-c] <OWL_ontology_file.owl>" << '\n'
      << od << '\n';
      return 0;
   }

   //create a triple store that is aware of standard OWL terms
   owlcpp::Triple_store store;

   const bfs::path in = vm["input-file"].as<std::string>();

   try {
      if( vm.count("include") ) { //load input-file and its includes
         owlcpp::Catalog cat;
         std::vector<std::string> const& vin = vm["include"].as<std::vector<std::string> >();
         if( vin.empty() ) {
            add(cat, in.parent_path(), true, 100);
         } else {
            BOOST_FOREACH(std::string const& p, vin) add(cat, p, true, 100);
         }
         load_file(in, store, cat);
      } else { //load just input-file
         load_file(in, store);
      }

      owlcpp::Triple_store::query_b<0,1,1,0>::range r = store.find_triple(
               owlcpp::any,
               owlcpp::terms::rdf_type::id(),
               owlcpp::terms::owl_Class::id(),
               owlcpp::any
      );

      if( vm.count("count") ) {
         std::cout
         << store.map_triple().size() << " triples" << '\n'
         << store.map_node().size() << " nodes" << '\n'
         << store.map_ns().size() << " namespace IRIs" << '\n'
         << boost::distance(r) << " owl:Class definitions" << '\n'
         ;
      } else {
         BOOST_FOREACH( owlcpp::Triple const& t, r ) {
            std::cout
            << '\"'
            << to_string(t.subj_, store) << "\"\t\""
            << to_string(t.pred_, store) << "\"\t\""
            << to_string(t.obj_, store) << "\"\t\n"
            ;
         }
      }

   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
