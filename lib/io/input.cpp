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
#include "owlcpp/io/catalog.hpp"
#include "raptor_to_store.hpp"

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
   detail::Raptor_to_store ats(store, path);
   ats.parse(stream);
}

/*
*******************************************************************************/
void load_file(boost::filesystem::path const& file, Triple_store& ts) {
   const std::string cp = canonical(file).string();
   detail::Raptor_to_store ats(ts, cp);
   ats.parse();
}

/*
*******************************************************************************/
void load_file(boost::filesystem::path const& file, Triple_store& ts, Catalog const& cat) {
   detail::Raptor_to_store ats(ts, file.string());
   ats.parse();
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

   detail::Check_both check(cat.ontology_iri(*did), cat.version_iri(*did));
   detail::Raptor_to_store rts(ts, cat.path(*did), check);
   rts.parse();
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
