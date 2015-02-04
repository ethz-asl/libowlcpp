/** @file "/owlcpp/apps/owlstats.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2015
*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include "boost/program_options.hpp"

#include "owlcpp/io/catalog.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

struct Node_stats : public owlcpp::Visitor_node {
   std::size_t n_blank_;
   std::size_t n_iri_;
   std::size_t n_bool_;
   std::size_t n_int_;
   std::size_t n_uns_;
   std::size_t n_float_;
   std::size_t n_str_;
   std::size_t str_len_;

   Node_stats()
   : n_blank_(0),
     n_iri_(0),
     n_bool_(0),
     n_int_(0),
     n_uns_(0),
     n_float_(0),
     n_str_(0),
     str_len_(0)
   {}

   void visit_impl(owlcpp::Node_iri const& node) { ++n_iri_;}
   void visit_impl(owlcpp::Node_blank const& node) {++n_blank_;}
   void visit_impl(owlcpp::Node_bool const& node) {++n_bool_;}
   void visit_impl(owlcpp::Node_int const& node) {++n_int_;}
   void visit_impl(owlcpp::Node_unsigned const& node) {++n_uns_;}
   void visit_impl(owlcpp::Node_double const& node) {++n_float_;}

   void visit_impl(owlcpp::Node_string const& node) {
      ++n_str_;
      str_len_ += node.value().size();
   }
};

/** Parse OWL ontology file and print triples
*******************************************************************************/
int main(int argc, char* argv[]) {
   bpo::options_description od;
   od.add_options()
            ("help,h", "help message")
            ("input-file", bpo::value<std::string>(), "input OWL file")
            ("include,i",
                     bpo::value<std::vector<std::string> >()->zero_tokens()
                     ->composing(), "search paths")
            ;
   bpo::positional_options_description pod;
   pod.add("input-file", -1);
   bpo::variables_map vm;
   store(
            bpo::command_line_parser(argc, argv)
               .options(od).positional(pod).run(),
            vm
   );
   notify(vm);

   if( ! vm.count("input-file") || vm.count("help") ) {
      std::cout
      << "Print OWL ontology statistics" << '\n'
      << "Usage:" << '\n'
      << "owlstats [-i[path]] <OWL_ontology_file.owl>" << '\n'
      << od << '\n';
      return ! vm.count("help");
   }

   owlcpp::Triple_store ts;
   const bfs::path in( vm["input-file"].as<std::string>());
   try {
      if( vm.count("include") ) { //load input-file and its includes
         owlcpp::Catalog cat;
         //how far to look into each file for ontologyVersion
         const std::size_t search_depth = 100;
         std::vector<std::string> const& vin =
                  vm["include"].as<std::vector<std::string> >();

         if( vin.empty() ) {
            add(cat, in.parent_path(), true, search_depth);
         } else {
            BOOST_FOREACH(std::string const& p, vin) {
               add(cat, p, true, search_depth);
            }
         }
         load_file(in, ts, cat);
      } else { //load just input-file
         load_file(in, ts);
      }
   } catch(...) {
      std::cerr
      << "error loading:" << '\n'
      << boost::current_exception_diagnostic_information()
      << std::endl;
      return 1;
   }

   Node_stats ns;
   BOOST_FOREACH(const owlcpp::Node_id nid, ts.map_node()) {
      ts[nid].accept(ns);
   }

   std::cout
   << "namespace IRIs: " << ts.map_ns().size() << '\n'
   << "IRI nodes: " << ns.n_iri_ << '\n'
   << "blank nodes: " << ns.n_blank_ << '\n'
   << "literal nodes:\n"
   << "   boolean nodes: " << ns.n_bool_ << '\n'
   << "   unsigned nodes: " << ns.n_uns_ << '\n'
   << "   integer nodes: " << ns.n_int_ << '\n'
   << "   float nodes: " << ns.n_float_ << '\n'
   << "   string nodes: " << ns.n_str_ << ", "
   << ((double)ns.str_len_ / ns.n_str_) << " average length\n"
   << "nodes total: " << ts.map_node().size() << '\n'
   << "triples: " << ts.map_triple().size() << '\n'
   ;

   try {

   } catch(...) {
      std::cerr
      << boost::current_exception_diagnostic_information()
      << std::endl;
      return 1;
   }
   return 0;
}
