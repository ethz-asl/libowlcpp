/** @file "/owl_cpp/apps/print_triples.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/print.hpp"

namespace oa = owl_cpp;

/** Parse single OWL ontology file into triple store and print triples
*******************************************************************************/
int main(int argc, char* argv[]) {
   if( argc < 2 ) {
      std::cout << "Usage: print_triples OWL_ontology_file.owl" << std::endl;
      return 0;
   }
   try {
      oa::Triple_store store;
      load(argv[1], store);
      print(store, std::cout);
   } catch(...) {
      std::cerr << b::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
