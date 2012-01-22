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
Doc_id Catalog::insert_doc(
         std::string const& path,
         std::string const& iri,
         std::string const& version
) {
   const Node_id iid = insert_iri_node(iri);
   if( version.empty() ) return doc_.insert(path, iid);
   const Node_id vid = insert_iri_node(version);
   return doc_.insert(path, iid, vid);
}

/*
*******************************************************************************/
Node const* Catalog::version(const Doc_id did) const {
   Node_id const* nid = doc_.version(did);
   if( nid ) return &node_[*nid];
   return 0;
}

/*
*******************************************************************************/
Node_id Catalog::insert_iri_node(std::string const& iri) {
   const std::size_t n = iri.find('#');
   if( std::string::npos == n ) {
      const Ns_id nid = iri_.insert(iri);
      return node_.insert(Node(nid, ""));
   }
   const Ns_id nid = iri_.insert(iri.substr(0,n));
   return node_.insert( Node(nid, iri.substr(n+1)) );
}

/*
*******************************************************************************/
Catalog& Catalog::add(boost::filesystem::path const& path, const bool recurse) {
   const unsigned char base_iri[] = "IRI not found";
   Rdf_parser parser = Rdf_parser::rdfxml(base_iri);
   detail::Iri_finder irif;
   boost::filesystem::ifstream is(path);
   parser(is, irif, 0);
   //TODO:
   return *this;
}


}//namespace owlcpp
