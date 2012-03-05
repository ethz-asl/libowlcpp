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

   owlcpp::Triple_store store;
   const bfs::path in( vm["input-file"].as<std::string>());
   try {
      if( vm.count("include") ) { //load input-file and its includes
         owlcpp::Catalog cat;
         std::vector<std::string> const& vin = vm["include"].as<std::vector<std::string> >();
         if( vin.empty() ) {
            cat.add(in.parent_path(), true, 100);
         } else {
            BOOST_FOREACH(std::string const& p, vin) cat.add(p, true, 100);
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

   namespace owl = owlcpp;
   namespace ot = owlcpp::terms;
   try {
/*
      boost::filesystem::path file(argv[1]);

      //catalog ontologies in the same directory
      owl::Catalog cat;
      owl::find_ontologies(cat, file.parent_path().string());

      //parse including imports
      owl::Triple_store store;
      load(argv[1], store, cat);

      std::cout << "Loaded " << store.n_triples() << " triples" << '\n';

      std::cout << "NAMESPACES:\n";
      owl::print_namespaces(store, std::cout);

      std::cout << "\nCLASSES:\n";

      //iterate over nodes
      BOOST_FOREACH( const owl::Node_id nid, store.node_ids() ) {
         //if declared as class, print
         if(
               owl::find_triples(
                     nid,
                     ot::T_rdf_type::id(),
                     ot::T_owl_Class::id(),
                     store
               )
         ) {
            std::cout << short_name(nid, store) << '\n';
         }
      }
*/
   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
