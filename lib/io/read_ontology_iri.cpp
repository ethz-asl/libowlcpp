/** @file "/owlcpp/lib/io/read_ontology_iri.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/read_ontology_iri.hpp"
#include "owlcpp/io/parser_triple.hpp"
#include "adaptor_iri_finder.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::pair<std::string,std::string> read_ontology_iri(boost::filesystem::path const& file) {
   Parser_triple parser;
   detail::Adaptor_iri_finder irif;
   //TODO: catch and re-throw file info

   parser(file.string(), irif);
   return make_pair(irif.iri(), irif.version());
}

}//namespace owlcpp
