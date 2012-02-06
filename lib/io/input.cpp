/** @file "/owlcpp/lib/io/input.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/input.hpp"

#include "boost/filesystem.hpp"

#include "owlcpp/io/read_ontology_iri.hpp"
#include "owlcpp/io/parser_triple.hpp"
#include "adaptor_triple_store.hpp"

namespace owlcpp {

/*
*******************************************************************************/
void load(std::istream& stream, Triple_store& store, std::string const& path) {
   Parser_triple parser;
   detail::Adaptor_triple_store ats(store, path);
   parser(stream, ats);
}

/*
*******************************************************************************/
void load_file(boost::filesystem::path const& file, Triple_store& ts) {
   const boost::filesystem::path cp = canonical(file);
   std::pair<std::string,std::string> pair;
   try{
      pair = read_ontology_iri(cp);
   } catch(Input_err const&) {
      //ignore
   }
   const Doc_id did = ts.insert_doc(cp.string(), pair.first, pair.second).first;
   Parser_triple parser;
   detail::Adaptor_triple_store ats(ts, file.string());
//   parser(is, ats);
   //TODO
}

/*
*******************************************************************************/
void load_file(boost::filesystem::path const& file, Triple_store& ts, Catalog const& cat) {
   //TODO
}

/*
*******************************************************************************/
void load_iri(std::string const& iri, Triple_store& ts, Catalog const& cat) {
   //TODO
}



}//namespace owlcpp
