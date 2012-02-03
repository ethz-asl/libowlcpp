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
   store_.insert(
            entry_t(Doc_id(), "", terms::T_empty_::id(), terms::T_empty_::id())
   );
   tracker_.ensure_min(Doc_id());
}

/*
*******************************************************************************/
std::pair<Doc_id,bool> Doc_map::insert(
         std::string const& path,
         const Node_id iri,
         const Node_id version
) {
   if( iri == terms::T_empty_::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("empty ontologyIRI is not allowed")
   );

   //multiple entries with empty paths are allowed
   if( path.empty() ) {
      iri_index_t const& ii = store_.get<iri_tag>();
      for(iri_iter_t i = ii.find(iri); i != ii.end(); ++i) {
         if( i->path_.empty() && i->version_id_ == version )
            return std::make_pair(i->id_, false);
      }
      return insert_private(path, iri, version);
   }

   path_index_t const& path_index = store_.get<path_tag>();
   path_iter_t path_iter = path_index.find(path);
   if( path_iter == path_index.end() ) return insert_private(path, iri, version);

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

   return std::make_pair(path_iter->id_, false);
}

/*
*******************************************************************************/
std::pair<Doc_id,bool> Doc_map::insert(std::string const& path, const Node_id iri) {
   return insert(path, iri, terms::T_empty_::id());
}

/*
*******************************************************************************/
std::pair<Doc_id,bool> Doc_map::insert_new() {
   return insert_private("", terms::T_empty_::id(), terms::T_empty_::id());
}

/*
*******************************************************************************/
void Doc_map::modify(
         const Doc_id did,
         std::string const& path,
         const Node_id iri,
         const Node_id version
) {
   BOOST_ASSERT(store_.get<id_tag>().find(did) != store_.get<id_tag>().end());

}

/*
*******************************************************************************/
void Doc_map::remove(const id_type id) {
   id_index_t & id_index = store_.get<id_tag>();
   id_iter_t i = id_index.find(id);
   BOOST_ASSERT( i != id_index.end() );
   id_index.erase(i);
   tracker_.push(id);
}

/*
*******************************************************************************/
Node_id Doc_map::iri(const id_type id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   return store_.get<id_tag>().find(id)->iri_id_;
}

/*
*******************************************************************************/
Node_id const* Doc_map::version(const id_type id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   const id_iter_t i = store_.get<id_tag>().find(id);
   if( i->version_id_ == terms::T_empty_::id() ) return 0;
   return &i->version_id_;
}

/*
*******************************************************************************/
std::string Doc_map::path(const id_type id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   return store_.get<id_tag>().find(id)->path_;
}

}//namespace owlcpp
