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
#include "boost/range/algorithm/copy.hpp"

#include "owlcpp/io/catalog.hpp"
#include "raptor_to_store.hpp"

namespace owlcpp {

/*
*******************************************************************************/
void load(
         std::istream& stream,
         Triple_store& store,
         std::string const& path,
         Check_id const& check
) {
   detail::Raptor_to_store rts(store, path, check);
   rts.parse(stream);
}

namespace {

/*
*******************************************************************************/
template<class Iter> void load(
         std::istream& stream,
         Triple_store& store,
         std::string const& path,
         Check_id const& check,
         Iter iter
) {
   detail::Raptor_to_store rts(store, path, check);
   rts.parse(stream);
   boost::copy(rts.imports(), iter);
}

}//namespace anonymous

/*
*******************************************************************************/
void load(
         std::istream& stream,
         Triple_store& store,
         Catalog const& cat,
         std::string const& path,
         Check_id const& check
) {
   std::vector<std::string> v;
   load(stream, store, path, check, back_inserter(v));
   try{
      BOOST_FOREACH(std::string const& iri, v) {
         if( store.find_doc_iri(iri) ) continue;
         load_iri(iri, store, cat);
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
void load_file(
         boost::filesystem::path const& file,
         Triple_store& store,
         Check_id const& check
) {
   const std::string cp = canonical(file).string();
   boost::filesystem::ifstream ifs(cp);
   load(ifs, store, cp, check);
}

/*
*******************************************************************************/
void load_file(
         boost::filesystem::path const& file,
         Triple_store& store,
         Catalog const& cat,
         Check_id const& check
) {
   const std::string cp = canonical(file).string();
   boost::filesystem::ifstream ifs(cp);
   load(ifs, store, cat, cp, check);
}

/*
*******************************************************************************/
void load_iri(std::string const& iri, Triple_store& store, Catalog const& cat) {
   if( Catalog::doc_version_range r = cat.find_doc_version(iri) ) {
      const Doc_id did = *r.begin();
      Check_both check(cat.ontology_iri_str(did), cat.version_iri_str(did));
      load_file(cat.path(did), store, cat, check);
   } else if(Catalog::doc_iri_range r = cat.find_doc_iri(iri)) {
      const Doc_id did = *r.begin();
      Check_both check(cat.ontology_iri_str(did), cat.version_iri_str(did));
      load_file(cat.path(did), store, cat, check);
   } else BOOST_THROW_EXCEPTION(
            Input_err()
            << Input_err::msg_t("ontology not found")
            << Input_err::str1_t(iri)
   );
}

}//namespace owlcpp
