/** @file "/owlcpp/lib/io/catalog.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/catalog.hpp"

#include "boost/filesystem.hpp"

#include "owlcpp/io/read_ontology_iri.hpp"
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp {
/*
*******************************************************************************/
std::string Catalog::ontology_iri_str(const Doc_id did) const {
   return to_string_full(operator[](ontology_iri_id(did)), *this);
}

/*
*******************************************************************************/
std::string Catalog::version_iri_str(const Doc_id did) const {
   return to_string_full(operator[](version_iri_id(did)), *this);
}

namespace{

/*
*******************************************************************************/
inline std::size_t add_to_catalog(
         boost::filesystem::path const& path,
         Catalog& cat,
         const std::size_t search_depth,
         const bool ignore_errors
) {
   const boost::filesystem::path cp = canonical(path);
   std::pair<std::string,std::string> pair;
   try{
      pair = read_ontology_iri(cp, search_depth);
   } catch(Input_err const&) {
      //ignore
      return 0;
   }
   if( pair.first.empty() ) {
      if(ignore_errors) return 0;
      BOOST_THROW_EXCEPTION(
               Input_err()
               << Input_err::msg_t("ontologyIRI not found")
               << Input_err::str1_t(path.string())
      );
   }
   return cat.insert_doc(pair.first, cp.string(), pair.second).second ? 1 : 0;
}

/*
*******************************************************************************/
template<class Iter> inline
std::size_t add_to_catalog(
         Iter i1, Iter i2, Catalog& cat, const std::size_t search_depth
) {
   std::size_t n = 0;
   for( ; i1 != i2; ++i1 ) {
      if( is_regular_file(*i1) )
         n += add_to_catalog(i1->path(), cat, search_depth, true);
   }
   return n;
}

}//namespace anonymous

/*
*******************************************************************************/
std::size_t add(
         Catalog& cat,
         boost::filesystem::path const& path,
         const bool recurse,
         const std::size_t search_depth
         ) {
   if( ! exists(path) ) BOOST_THROW_EXCEPTION(
            Input_err()
            << Input_err::msg_t("not found")
            << Input_err::str1_t(path.string())
   );
   if( is_directory(path) ) {
      if( recurse ) {
         boost::filesystem::recursive_directory_iterator i1(path), i2;
         return add_to_catalog(i1, i2, cat, search_depth);
      } else {
         boost::filesystem::directory_iterator i1(path), i2;
         return add_to_catalog(i1, i2, cat, search_depth);
      }
   } else if( is_regular_file(path) ) {
      return add_to_catalog(path, cat, search_depth, false);
   }
   return 0;
}

}//namespace owlcpp
