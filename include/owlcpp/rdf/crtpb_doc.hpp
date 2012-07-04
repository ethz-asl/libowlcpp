/** @file "/owlcpp/include/owlcpp/rdf/crtpb_doc.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CRTPB_DOC_HPP_
#define CRTPB_DOC_HPP_
#include <string>
#include "boost/assert.hpp"

#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/** Enable interaction between a map of ontology documents and
a map of IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Crtpb_doc {
   typedef typename Map_traits<Super>::map_node_t map_node_t;
   typedef typename Map_traits<Super>::map_doc_t map_doc_t;

   map_doc_t const& doc() const {
      return static_cast<Super const&>(*this).docs();
   }

   map_doc_t& doc() {
      return static_cast<Super&>(*this).doc_;
   }

public:

   Doc_meta const& operator[](const Doc_id did) const {
      return doc()[did];
   }

   Doc_meta const& at(const Doc_id did) const {
      return doc().at(did);
   }

   /**@brief Add document info: location, ontologyIRI, and versionIRI.
    @param iri ontologyIRI
    @param path document location
    @param vers versionIRI
    @return document ID and whether new document info was actually added
    @throw Err if an entry with the same non-empty @a path and different @a iri or @a version
    is already present.
    @details
    Duplicate document info entries are not allowed.

    If document info with same @a path, @a iri, and @a version already present,
    new info is not inserted and the ID of the existing document is returned.

    Since documents with same ontologyIRI and versionIRI may be found
    at different paths,
    multiple entries with different @a path and same @a iri or @a version can be added.

    Sometimes, document path is not known.
    Therefore multiple entries with empty @a path and different @a iri or @a version
    are allowed.
   */
   std::pair<Doc_id,bool> insert_doc(
            const Node_id iri,
            std::string const& path = "",
            const Node_id vers = terms::T_empty_::id()
   ) {
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(iri) && "invalid ontology IRI ID" );
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(vers) && "invalid version IRI ID" );
      return doc().insert(iri, path, vers);
   }

   std::pair<Doc_id,bool> insert_doc(
            std::string const& iri,
            std::string const& path = "",
            std::string const& vers = ""
   ) {
      const Node_id iri_id = static_cast<Super&>(*this).insert_node_iri(iri);
      const Node_id vers_id = static_cast<Super&>(*this).insert_node_iri(vers);
      return insert_doc(iri_id, path, vers_id);
   }

   typename map_doc_t::iri_range find_doc_iri(const Node_id iri) const {
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(iri) && "invalid ontology IRI ID" );
      return doc().find_iri(iri);
   }

   typename map_doc_t::iri_range find_doc_iri(std::string const& iri) const {
      if(
               Node_id const*const nid =
                        static_cast<Super const&>(*this).find_node_iri(iri)
      ) return doc().find_iri(nid);
      return typename map_doc_t::iri_range();
   }
};

}//namespace owlcpp
#endif /* CRTPB_DOC_HPP_ */
