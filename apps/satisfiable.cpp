/** @file "/owl_cpp/apps/satisfiable.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include "boost/exception/diagnostic_information.hpp"
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include "factpp/Kernel.hpp"

#include "owl_cpp/rdf/triple_store.hpp"
#include "owl_cpp/rdf/query_triples.hpp"
#include "owl_cpp/io/parse_to_triple_store.hpp"
#include "owl_cpp/reasoner/triple_to_fact.hpp"
#include "owl_cpp/reasoner/query_fact.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/terms/term_tags.hpp"

/**
Parse OWL ontology file and its imports located in the same folder
Load ontology to FaCT++ reasoner
check if all classes are satisfiable
*******************************************************************************/
int main(int argc, char* argv[]) {
   namespace owl = owl_cpp;
   namespace ot = owl_cpp::terms;
   if( argc < 2 ) {
      std::cout
      <<
      "Load OWL ontology file including imports found in the same folder "
      "or in optional additional locations. "
      "Check every class for satisfiability." "\n"
      "Usage: satisfiable file.owl [location ...]"
      << std::endl;
      return 0;
   }
   try {
      owl::Catalog cat;
      if( argc == 2 ) {
         //catalog ontologies in the same directory
         boost::filesystem::path file(argv[1]);
         owl::find_ontologies(cat, file.parent_path().string());
      } else {
         //catalog ontologies in specified locations
         for(int i = 1; i != argc; ++i) owl::find_ontologies(cat, argv[i]);
      }

      //parse including imports
      owl::Triple_store store;
      load(argv[1], store, cat);

      ReasoningKernel kernel;
      add(store, kernel);

      if( ! kernel.isKBConsistent() ) {
         std::cout << "inconsistent ontology";
         return 1;
      }

      bool all_satisfiable = true;
      //iterate over nodes
      BOOST_FOREACH( const owl::Node_id nid, store.node_ids() ) {
         //find nodes declared as classes
         if(
               owl::find_triples(
                     nid,
                     ot::T_rdf_type::id(),
                     ot::T_owl_Class::id(),
                     store
               )
         ) {
            const TDLConceptExpression* ce = owl::concept(nid, store, kernel);
            if( ! kernel.isSatisfiable(ce) ) {
               std::cout
               << short_name(nid, store) << '\t'
               << '"' << find_label(nid, store) << '"' << '\t'
               << "not satisfiable" << '\n';
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
