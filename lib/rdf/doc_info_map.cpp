/** @file "/owlcpp/lib/rdf/doc_info_map.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "boost/assert.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp {

/*
*******************************************************************************/
Doc_map::Doc_map() : tracker_(), store_() {
//   store_.insert(
//            entry_t(Doc_id(), "", terms::T_empty_::id(), terms::T_empty_::id())
//   );
   tracker_.ensure_min(Doc_id());
}

/*
*******************************************************************************/
Doc_id const* Doc_map::find_existing(
         std::string const& path,
         const Node_id iri,
         const Node_id version
) const {
   if( iri == terms::T_empty_::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("empty ontologyIRI is not allowed")
   );

   //multiple entries with empty paths are allowed
   if( path.empty() ) {
      iri_index_t const& ii = store_.get<iri_tag>();
      for(iri_iter_t i = ii.find(iri); i != ii.end(); ++i) {
         if( i->path_.empty() && i->version_id_ == version )
            return &i->id_;
      }
      return 0;
   }

   path_index_t const& path_index = store_.get<path_tag>();
   path_iter_t path_iter = path_index.find(path);
   if( path_iter == path_index.end() ) return 0;

   if( path_iter->iri_id_ != iri ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("different IRI at same location")
            << Err::str1_t(path)
            << Err::int1_t(iri())
            << Err::int2_t(path_iter->iri_id_())
   );
   if( path_iter->version_id_ != version ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("different ontology version in same location")
            << Err::str1_t(path)
            << Err::int1_t(version())
            << Err::int2_t(path_iter->version_id_())
   );

   return &path_iter->id_;
}

/*
*******************************************************************************/
std::pair<Doc_id,bool> Doc_map::insert(std::string const& path, const Node_id iri) {
   return insert(path, iri, terms::T_empty_::id());
}

/*
*******************************************************************************/
Doc_id Doc_map::insert_new() {
   return insert_private("", terms::T_empty_::id(), terms::T_empty_::id());
}

/*
*******************************************************************************/
void Doc_map::remove(const Doc_id id) {
   id_index_t & id_index = store_.get<id_tag>();
   id_iter_t i = id_index.find(id);
   BOOST_ASSERT( i != id_index.end() );
   id_index.erase(i);
   tracker_.push(id);
}

/*
*******************************************************************************/
Node_id Doc_map::iri(const Doc_id id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   return store_.get<id_tag>().find(id)->iri_id_;
}

/*
*******************************************************************************/
Node_id Doc_map::version(const Doc_id id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   const id_iter_t i = store_.get<id_tag>().find(id);
   return i->version_id_;
}

/*
*******************************************************************************/
std::string Doc_map::path(const Doc_id id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   return store_.get<id_tag>().find(id)->path_;
}

}//namespace owlcpp
