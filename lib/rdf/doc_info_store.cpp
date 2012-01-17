/** @file "/owlcpp/lib/rdf/doc_info_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "boost/assert.hpp"
#include "owlcpp/rdf/doc_info_store.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp {

/*
*******************************************************************************/
Doc_store::id_type Doc_store::insert(std::string const& path, const Node_id iri, const Node_id ver) {
   path_index_t const& path_index = store_.get<path_tag>();
   path_iter_t path_iter = path_index.find(path);

   if( path_iter == path_index.end() ) {
      const id_type id = tracker_.get();
      BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
      store_.insert(entry_t(id, path, iri, ver));
      return id;
   }
   if( path_iter->iri_id_ != iri )
      BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("different IRI in same location")
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

   return path_iter->id_;
}

/*
*******************************************************************************/
Doc_store::id_type Doc_store::insert(std::string const& path, const Node_id iri) {
   return insert(path, iri, terms::T_empty_::id());
}

/*
*******************************************************************************/
Node_id Doc_store::iri(const id_type id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   return store_.get<id_tag>().find(id)->iri_id_;
}

/*
*******************************************************************************/
Node_id Doc_store::version(const id_type id) const {
   BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
   return store_.get<id_tag>().find(id)->version_id_;
}

/*
Doc_store::id_type const* Doc_store::find(const Node_id id) const {
   version_index_t const& v_ind = store_.get<version_tag>();
   version_iter_t v_iter = v_ind.find(id);
   if( v_iter != v_ind.end() ) return &v_iter->id_;

   iri_index_t const& iri_ind = store_.get<iri_tag>();
   iri_iter_t iri_iter = iri_ind.find(id);
   if( iri_iter != iri_ind.end() ) return &iri_iter->id_;

   return 0;
}
*******************************************************************************/

}//namespace owlcpp
