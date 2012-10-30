/** @file "/owlcpp/lib/io/read_ontology_iri.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "boost/filesystem/fstream.hpp"
#include "owlcpp/io/read_ontology_iri.hpp"
#include "owlcpp/io/raptor_wrapper.hpp"
#include "raptor_to_iri.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::pair<std::string,std::string> read_ontology_iri(
         std::istream& is,
         const std::size_t search_depth
) {
   Raptor_wrapper parser;
   detail::Raptor_to_iri rti(parser.abort_call(), search_depth);
   parser(is, rti);
   return make_pair(rti.iri(), rti.version());
}

/*
*******************************************************************************/
std::pair<std::string,std::string> read_ontology_iri(
         boost::filesystem::path const& file,
         const std::size_t search_depth
) {
   boost::filesystem::ifstream ifs(file);
   return read_ontology_iri(ifs, search_depth);
}

}//namespace owlcpp
