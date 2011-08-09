/** @file "/owl_cpp/apps/validate.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include "boost/exception/diagnostic_information.hpp"
namespace b = boost;
#include "boost/filesystem.hpp"
namespace bf = boost::filesystem;
#include "Kernel.h"

#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_to_fact.hpp"
namespace oa = owl_cpp;
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

/**
Parse OWL ontology file and its imports located in the same folder
Load ontology to FaCT++ reasoner and check if it is consistent
*******************************************************************************/
int main(int argc, char* argv[]) {
   if( argc < 2 ) {
      std::cout << "Usage: validate ontology_file.owl" << std::endl;
      return 0;
   }
   try {
      bf::path file(argv[1]);

      //catalog ontologies in the same directory
      oa::Catalog cat;
      oa::find_ontologies(cat, file.parent_path().string());

      //parse including imports
      oa::Triple_store store;
      load(argv[1], store, cat);

      ReasoningKernel kernel;
      add(store, kernel);

      if( kernel.isKBConsistent() ) {
         std::cout << "ontology is consistent" << std::endl;
         return 0;
      } else {
         std::cout << "ontology is inconsistent" << std::endl;
         return 1;
      }

   } catch(...) {
      std::cerr << b::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
