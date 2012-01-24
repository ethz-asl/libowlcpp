/** @file "/owlcpp/lib/io/catalog.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/catalog.hpp"

#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem.hpp"

#include "parse.hpp"
#include "adaptor_iri_finder.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::pair<std::string,std::string> ontology_id(boost::filesystem::path const& file) {
   Rdf_parser parser = Rdf_parser::rdfxml("IRI not found");
   detail::Iri_finder irif;
   boost::filesystem::ifstream is(file);
   //TODO: catch and re-throw file info
   parser(is, irif, 0);
   return make_pair(irif.iri(), irif.version());
}

namespace{
/*
*******************************************************************************/
inline std::size_t add_to_catalog(boost::filesystem::path const& path, Catalog& cat) {
   const boost::filesystem::path cp = canonical(path);
   try{
      const std::pair<std::string,std::string> pair = ontology_id(cp);
      return cat.insert_doc(cp.string(), pair.first, pair.second).second ? 1 : 0;
   } catch(Rdf_parser::Err const&) {
      //ignore
   }
   return 0;
}

template<class Iter> inline
std::size_t add_to_catalog(Iter i1, Iter i2, Catalog& cat) {
   std::size_t n = 0;
   for( ; i1 != i2; ++i1 ) {
      if( is_regular_file(*i1) ) n += add_to_catalog(i1->path(), cat);
   }
   return n;
}

}//namespace anonymous

/*
*******************************************************************************/
std::size_t Catalog::add(boost::filesystem::path const& path, const bool recurse) {
   if( ! exists(path) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("not found")
            << Err::str1_t(path.string())
   );
   if( is_directory(path) ) {
      if( recurse ) {
         boost::filesystem::recursive_directory_iterator i1(path), i2;
         return add_to_catalog(i1, i2, *this);
      } else {
         boost::filesystem::directory_iterator i1(path), i2;
         return add_to_catalog(i1, i2, *this);
      }
   } else if( is_regular_file(path) ) {
      return add_to_catalog(path, *this);
   }
   return 0;
}

}//namespace owlcpp
