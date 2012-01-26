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
         const Node_id ver
) {
   path_index_t const& path_index = store_.get<path_tag>();
   path_iter_t path_iter = path_index.find(path);

   if( path_iter == path_index.end() ) {
      const id_type id = tracker_.get();
      BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
      store_.insert(entry_t(id, path, iri, ver));
      return std::make_pair(id, true);
   }
   //TODO: allow duplicate empty path
   if( path_iter->iri_id_ != iri )
      BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("different IRI at same location")
            << Err::str1_t(path)
            << Err::int1_t(iri())
            << Err::int2_t(path_iter->iri_id_())
      );
   if( path_iter->version_id_ != ver )
      BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("different ontology version in same location")
            << Err::str1_t(path)
            << Err::int1_t(ver())
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
