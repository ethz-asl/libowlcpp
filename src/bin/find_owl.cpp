/** @file "/owlcpp/apps/find_ontologies.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include "boost/foreach.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/lib_info.hpp"

/**
Make a catalog of OWL ontologies in input locations
*******************************************************************************/
int main(int argc, char* argv[]) {
   owlcpp::Lib_info::print(std::cout << "find_owl\n" "built with ") << std::endl;
   if( argc < 2 ) {
      std::cout << "Usage: find_owl path1 [path2 ...]" << std::endl;
      return 0;
   }
   try {
      owlcpp::Catalog cat;
      for(int i = 1; i != argc; ++i) add(cat, argv[i], false, 100);
      std::cout << "path" << '\t' << "ontologyIRI" << '\t' << "versionIRI" << '\n';
      BOOST_FOREACH(const owlcpp::Doc_id id, cat) {
         std::cout
         << '\"' << cat.path(id) << "\"\t\""
         << cat.ontology_iri_str(id) << "\"\t\""
         << cat.version_iri_str(id) << "\"\n"
         ;
      }
   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
