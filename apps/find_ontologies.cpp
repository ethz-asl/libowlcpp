/** @file "/owl_cpp/apps/find_ontologies.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include "boost/exception/diagnostic_information.hpp"
#include "owl_cpp/io/parse_to_triple_store.hpp"

/**
Make a catalog of OWL ontologies in input locations
*******************************************************************************/
int main(int argc, char* argv[]) {
   namespace owl = owl_cpp;
   if( argc < 2 ) {
      std::cout << "Usage: find_owl path1 [path2 ...]" << std::endl;
      return 0;
   }
   try {
      owl::Catalog cat;
      for(int i = 1; i != argc; ++i) owl::find_ontologies(cat, argv[i]);

      typedef owl::Catalog::location_range_t range_t;
      range_t r = cat.locations();
      for(range_t::iterator i = r.begin(); i != r.end(); ++i)
         std::cout << i->path << '\t' << i->iri << '\t' << i->version << '\n';

   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
