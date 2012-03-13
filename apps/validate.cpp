/** @file "/owlcpp/apps/validate.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include "boost/exception/diagnostic_information.hpp"
#include "boost/filesystem.hpp"
#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "owlcpp/terms/term_tags.hpp"

/**
Parse OWL ontology file and its imports located in the same folder
Load ontology to FaCT++ reasoner and check if it is consistent
*******************************************************************************/
int main(int argc, char* argv[]) {
   namespace owl = owlcpp;
   namespace ot = owlcpp::terms;
   if( argc < 2 ) {
      std::cout << "Usage: validate ontology_file.owl" << std::endl;
      return 0;
   }
   try {
      boost::filesystem::path file(argv[1]);

      //catalog ontologies in the same directory
      owl::Catalog cat;
      owl::find_ontologies(cat, file.parent_path().string());

      //parse including imports
      owl::Triple_store store;
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
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
