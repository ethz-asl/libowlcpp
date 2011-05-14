/** @file "/owl_cpp/apps/find_ontologies.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#include <iostream>
#include "boost/exception/diagnostic_information.hpp"
namespace b = boost;
#include "owl_cpp/parse_to_triple_store.hpp"
namespace oa = owl_cpp;

/**
Make a catalog of OWL ontologies in input locations
*******************************************************************************/
int main(int argc, char* argv[]) {
   if( argc < 2 ) {
      std::cout << "Usage: find_owl path1 [path2 ...]" << std::endl;
      return 0;
   }
   try {
      oa::Catalog cat;
      for(int i = 1; i != argc; ++i) oa::find_ontologies(cat, argv[i]);

      typedef oa::Catalog::location_range_t range_t;
      range_t r = cat.locations();
      for(range_t::iterator i = r.begin(); i != r.end(); ++i)
         std::cout << i->path << '\t' << i->iri << '\t' << i->version << '\n';

   } catch(...) {
      std::cerr << b::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
