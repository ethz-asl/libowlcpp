/** @file "/owl_cpp/apps/satisfiable.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include "boost/exception/diagnostic_information.hpp"
namespace b = boost;
#include "boost/filesystem.hpp"
namespace bf = boost::filesystem;
#include "boost/foreach.hpp"
#include "factpp/Kernel.hpp"

#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_to_fact.hpp"
#include "owl_cpp/query_triples.hpp"
#include "owl_cpp/query_fact.hpp"
#include "owl_cpp/print.hpp"
namespace oa = owl_cpp;
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

/**
Parse OWL ontology file and its imports located in the same folder
Load ontology to FaCT++ reasoner
check if all classes are satisfiable
*******************************************************************************/
int main(int argc, char* argv[]) {
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
      oa::Catalog cat;
      if( argc == 2 ) {
         //catalog ontologies in the same directory
         bf::path file(argv[1]);
         oa::find_ontologies(cat, file.parent_path().string());
      } else {
         //catalog ontologies in specified locations
         for(int i = 1; i != argc; ++i) oa::find_ontologies(cat, argv[i]);
      }

      //parse including imports
      oa::Triple_store store;
      load(argv[1], store, cat);

      ReasoningKernel kernel;
      add(store, kernel);

      if( ! kernel.isKBConsistent() ) {
         std::cout << "inconsistent ontology";
         return 1;
      }

      bool all_satisfiable = true;
      //iterate over nodes
      BOOST_FOREACH( const oa::Node_id nid, store.node_ids() ) {
         //find nodes declared as classes
         if(
               oa::find_triples(
                     nid,
                     ot::T_rdf_type::id(),
                     ot::T_owl_Class::id(),
                     store
               )
         ) {
            const TDLConceptExpression* ce = oa::concept(nid, store, kernel);
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
      std::cerr << b::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
