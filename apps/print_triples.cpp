/** @file "/owlcpp/apps/print_triples.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/print.hpp"

/** Parse single OWL ontology file into triple store and print triples
*******************************************************************************/
int main(int argc, char* argv[]) {
   if( argc < 2 ) {
      std::cout << "Usage: print_triples OWL_ontology_file.owl" << std::endl;
      return 0;
   }
   try {
      owlcpp::Triple_store store;
      load(argv[1], store);
      print(store, std::cout);
   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
