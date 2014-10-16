/** @file "/owlcpp/apps/satisfiable.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "boost/exception/diagnostic_information.hpp"
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include "boost/program_options.hpp"
#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace bpo = boost::program_options;

/**
Parse OWL ontology file and its imports located in the same folder.
Load ontology to FaCT++ reasoner and check if all classes are satisfiable
*******************************************************************************/
int main(int argc, char* argv[]) {
   bpo::options_description od;
   od.add_options()
   ("help,h", "help message")
   ("input-file", bpo::value<std::string>(), "input OWL file")
   ("include,i",
          bpo::value<std::vector<std::string> >()->zero_tokens()->composing(),
          "search paths")
   ("strict", bpo::bool_switch()->default_value(true), "strict parsing")
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
      <<
      "Load OWL ontology file including imports found in the same folder "
      "or in optional additional locations. "
      "Check every class for satisfiability." "\n"
      << "Usage:" << '\n'
      << "satisfiable [-i[path]] [-c] <OWL_ontology_file.owl>" << '\n'
      << od << '\n';
      return ! vm.count("help");
   }

   //create a triple store
   owlcpp::Triple_store store;

   const boost::filesystem::path in = vm["input-file"].as<std::string>();

   try {
      if( vm.count("include") ) { //load input-file and its includes
         owlcpp::Catalog cat;
         std::vector<std::string> const& vin =
                  vm["include"].as<std::vector<std::string> >();
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
      submit(store, kernel, vm["strict"].as<bool>());

      if( ! kernel.isKBConsistent() ) {
         std::cout << "inconsistent ontology";
         return 1;
      }

      TExpressionManager& em = *kernel.getExpressionManager();
      bool all_satisfiable = true;
      //iterate over nodes
      BOOST_FOREACH( const owlcpp::Node_id nid, store.map_node() ) {
         //find nodes declared as classes
         if(
                  store.find_triple(
                           nid,
                           owlcpp::terms::rdf_type::id(),
                           owlcpp::terms::owl_Class::id(),
                           owlcpp::any
                  )
         ) {
            const TDLConceptExpression* ce =
                     em.Concept(to_string_full(nid, store));
            if( ! kernel.isSatisfiable(ce) ) {
               std::cout
               << to_string(nid, store) << '\t'
               << '"' << find_label(nid, store) << '"' << '\t'
               << "not satisfiable" << '\n'
               ;
               all_satisfiable = false;
            }
         }
      }

      if( all_satisfiable ) {
         std::cout << "all classes satisfiable";
         return 0;
      } else {
         return 1;
      }

   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
