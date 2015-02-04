/** @file "/owlcpp/include/owlcpp/rdf/map_doc_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_DOC_CRTPB_HPP_
#define MAP_DOC_CRTPB_HPP_
#include "boost/assert.hpp"

#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/rdf/store_concepts.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/** Enable interaction between a map of ontology documents and
a map of IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_doc_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_node_type map_node_t;
   typedef typename traits::map_doc_type map_doc_type;
   typedef typename map_doc_type::Err Err;
   typedef typename traits::doc_type doc_type;

   map_doc_type const& _map_doc() const {
      return static_cast<Super const&>(*this).map_doc_;
   }

   map_doc_type& _map_doc() {
      return static_cast<Super&>(*this).map_doc_;
   }

public:
   typedef typename map_doc_type::iri_range doc_iri_range;
   typedef typename map_doc_type::version_range doc_version_range;

   doc_type const& operator[](const Doc_id did) const {
      return _map_doc()[did];
   }

   doc_type const& at(const Doc_id did) const {
      return _map_doc().at(did);
   }

   doc_type const* find(const Doc_id did) const {
      return _map_doc().find(did);
   }

   doc_iri_range find_doc_iri(std::string const& iri) const {
      BOOST_CONCEPT_ASSERT((Ns_iri_node_store<Super>));
      Super const& super = static_cast<Super const&>(*this);
      Node_id const*const nid = super.find_node_iri(iri);
      if( nid ) return _map_doc().find_iri(*nid);
      return doc_iri_range(_map_doc().end_iri(), _map_doc().end_iri());
   }

   doc_version_range find_doc_version(std::string const& version) const {
      BOOST_CONCEPT_ASSERT((Ns_iri_node_store<Super>));
      Super const& super = static_cast<Super const&>(*this);
      Node_id const*const nid = super.find_node_iri(version);
      if( nid ) return _map_doc().find_version(*nid);
      return doc_version_range(
               _map_doc().end_version(),
               _map_doc().end_version()
      );
   }

   /**@brief Add document info: location, ontologyIRI, and versionIRI.
    @param iri_id ontologyIRI
    @param path document location
    @param vers_id versionIRI
    @return document ID and whether new document info was actually added
    @throw Err if an entry with the same non-empty @a path and different
    @a iri or @a version is already present.
    @details
    Duplicate document info entries are not allowed.

    If document info with same @a path, @a iri, and @a version already present,
    new info is not inserted and the ID of the existing document is returned.

    Since documents with same ontologyIRI and versionIRI may be found
    at different paths,
    multiple entries with different @a path and same @a iri or @a version
    can be added.

    Sometimes, document path is not known.
    Therefore multiple entries with empty @a path and different @a iri or
    @a version are allowed.
   */
   std::pair<Doc_id,bool> insert_doc(
            const Node_id iri_id,
            std::string const& path = "",
            const Node_id vers_id = terms::empty_::id()
   ) {
      BOOST_CONCEPT_ASSERT((Iri_node_store<Super>));
      BOOST_ASSERT(
               static_cast<Super const&>(*this).find(iri_id) &&
               "invalid ontology IRI ID"
      );

      BOOST_ASSERT(
               static_cast<Super const&>(*this).find(vers_id) &&
               "invalid version IRI ID"
      );

      return _map_doc().insert(iri_id, path, vers_id);
   }

   std::pair<Doc_id,bool> insert_doc(
            std::string const& iri,
            std::string const& path = "",
            std::string const& vers = ""
   ) {
      BOOST_CONCEPT_ASSERT((Ns_iri_node_store<Super>));
      const Node_id iri_id = static_cast<Super&>(*this).insert_node_iri(iri);
      const Node_id vers_id = static_cast<Super&>(*this).insert_node_iri(vers);
      try{
         return insert_doc(iri_id, path, vers_id);
      } catch(Err const& ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("error adding document")
                  << typename Err::str1_t(iri)
                  << typename Err::str2_t(path)
                  << typename Err::str3_t(vers)
                  << typename Err::nested_t(boost::current_exception())
         );
      }
   }
};

}//namespace owlcpp
#endif /* MAP_DOC_CRTPB_HPP_ */
