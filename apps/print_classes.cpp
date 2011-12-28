/** @file "/owl_cpp/apps/print_classes.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include <iostream>
#include <string>
#include "boost/exception/diagnostic_information.hpp"
#include "boost/range.hpp"
#include "boost/foreach.hpp"
#include "boost/filesystem.hpp"
#include "owl_cpp/rdf/triple_store.hpp"
#include "owl_cpp/rdf/query_triples.hpp"
#include "owl_cpp/io/parse_to_triple_store.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/terms/term_tags.hpp"

/**
Parse OWL ontology file and its imports located in the same folder
Find triples that define classes
Print prefixed names of classes
*******************************************************************************/
int main(int argc, char* argv[]) {
   namespace owl = owl_cpp;
   namespace ot = owl_cpp::terms;
   if( argc < 2 ) {
      std::cout << "Usage: print_classes ontology_file.owl" << std::endl;
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

      std::cout << "Loaded " << store.n_triples() << " triples" << '\n';

      std::cout << "NAMESPACES:\n";
      owl::print_namespaces(store, std::cout);

      std::cout << "\nCLASSES:\n";

      //iterate over nodes
      BOOST_FOREACH( const owl::Node_id nid, store.node_ids() ) {
         //if declared as class, print
         if(
               owl::find_triples(
                     nid,
                     ot::T_rdf_type::id(),
                     ot::T_owl_Class::id(),
                     store
               )
         ) {
            std::cout << short_name(nid, store) << '\n';
         }
      }
   } catch(...) {
      std::cerr << boost::current_exception_diagnostic_information() << std::endl;
      return 1;
   }
   return 0;
}
