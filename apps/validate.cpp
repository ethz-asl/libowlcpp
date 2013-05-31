/** @file "/owlcpp/apps/validate.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include "boost/program_options.hpp"
#include "boost/range.hpp"
#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

/**
Parse OWL ontology file and its imports located in the same folder.
Load ontology to FaCT++ reasoner and check if it is consistent
*******************************************************************************/
int main(int argc, char* argv[]) {
   bpo::options_description od;
   od.add_options()
   ("help,h", "help message")
   ("input-file", bpo::value<std::string>(), "input OWL file")
   ("include,i",
            bpo::value<std::vector<std::string> >()->zero_tokens()->composing(),
            "search paths")
   ("diagnose,d", bpo::bool_switch(),
            "check for reasoning errors after each axiom ")
   ("lax", bpo::bool_switch(), "non-strict parsing")
   ("return-success,S", bpo::bool_switch(),
            "return 1 if ontology is not consistent")
   ;
   bpo::positional_options_description pod;
   pod.add("input-file", -1);
   bpo::variables_map vm;
   store(bpo::command_line_parser(argc, argv).options(od).positional(pod).run(), vm);
   notify(vm);

   if( ! vm.count("input-file") || vm.count("help") ) {
      std::cout
      << "Check if ontology is consistent" << '\n'
      << "Usage:" << '\n'
      << "validate [-i[path]] [-c] <OWL_ontology_file.owl>" << '\n'
      << od << '\n';
      return ! vm.count("help");
   }

   //create a triple store
   owlcpp::Triple_store store;

   const bfs::path in = vm["input-file"].as<std::string>();

   try {
      if( vm.count("include") ) { //load input-file and its includes
         owlcpp::Catalog cat;
         std::vector<std::string> const& vin = vm["include"].as<std::vector<std::string> >();
         if( vin.empty() ) {
            add(cat, in.parent_path(), true);
         } else {
            BOOST_FOREACH(std::string const& p, vin) add(cat, p, true);
         }
         load_file(in, store, cat);
      } else { //load just input-file
         load_file(in, store);
      }

      ReasoningKernel kernel;
      const std::size_t n = submit(
               store,
               kernel,
               vm["lax"].as<bool>(),
               vm["diagnose"].as<bool>()
      );

      std::cout << n << " axioms generated" << std::endl;
      const bool consistent = kernel.isKBConsistent();
      std::cout
      << '\n' << "ontology is "
      << (consistent ? "consistent" : "inconsistent")
      << std::endl
      ;
      return vm["return-success"].as<bool>() && ! consistent;
   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
}
