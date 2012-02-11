/** @file "/owlcpp/lib/io/input.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/input.hpp"

#include <iostream>
#include <set>
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"

#include "owlcpp/io/read_ontology_iri.hpp"
#include "owlcpp/io/parser_triple.hpp"
#include "owlcpp/io/catalog.hpp"
#include "adaptor_triple_store.hpp"

namespace owlcpp { namespace{
/*
*******************************************************************************/
void load_file(
         std::string const& path,
         Triple_store& ts,
         std::set<std::string>& imports,
         std::string const& iri,
         std::string const& version
) {
//   if( iri.impty() )
//   detail::Adaptor_triple_store ats(ts, path);
}

}//namespace anonymous

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
   const std::string cp = canonical(file).string();
   Parser_triple parser;
   detail::Adaptor_triple_store ats(ts, cp);
   parser(cp, ats);
}

/*
*******************************************************************************/
void load_file(boost::filesystem::path const& file, Triple_store& ts, Catalog const& cat) {
   detail::Adaptor_triple_store ats(ts, file.string());
   Parser_triple parser;
   parser(file.string(), ats);

   try{
      BOOST_FOREACH(std::string const& iiri, ats.imports()) {
         if( ts.find_doc_iri(iiri) ) continue;
         load_iri(iiri, ts, cat);
      }
   } catch(Input_err&) {
      BOOST_THROW_EXCEPTION(
                  Input_err()
                  << Input_err::msg_t("import error")
                  << Input_err::nested_t(boost::current_exception())
      );
   }
}

/*
*******************************************************************************/
void load_iri(std::string const& iri, Triple_store& ts, Catalog const& cat) {
   Doc_id const* did = cat.find_doc_iri(iri);
   if( ! did ) BOOST_THROW_EXCEPTION(
            Input_err()
            << Input_err::msg_t("ontology not found")
            << Input_err::str1_t(iri)
   );

   detail::Adaptor_triple_store ats(ts, cat.path(*did), cat.iri(*did), cat.version(*did));
   Parser_triple parser;
   std::cout << cat.path(*did) << std::endl;
//   boost::filesystem::ifstream ifs(cat.path(*did));
//   parser(ifs, ats);
   parser(cat.path(*did), ats);

   try{
      BOOST_FOREACH(std::string const& iiri, ats.imports()) {
         if( ts.find_doc_iri(iiri) ) continue;
         load_iri(iiri, ts, cat);
      }
   } catch(Input_err&) {
      BOOST_THROW_EXCEPTION(
                  Input_err()
                  << Input_err::msg_t("import error")
                  << Input_err::nested_t(boost::current_exception())
      );
   }
}



}//namespace owlcpp
