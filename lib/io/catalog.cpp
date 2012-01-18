/** @file "/owlcpp/lib/io/catalog.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/catalog.hpp"

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



}//namespace owlcpp
