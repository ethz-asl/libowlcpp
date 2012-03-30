/** @file "/owlcpp/include/owlcpp/rdf/store_doc_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_DOC_CRTPB_HPP_
#define STORE_DOC_CRTPB_HPP_
#include "owlcpp/rdf/doc_info_map.hpp"

namespace owlcpp{

/**@brief CRTP base for document info stores
*******************************************************************************/
template<class T> class Store_doc_crtpb {
public:
   typedef Doc_map doc_map_t;

   doc_map_t const& documents() const {return docs_;}

   /**
    @param iri OntologyIRI or VersionIRI
    @return pointer to document ID for the first document that has specified VersionIRI or,
    if not found, for the first document that has specified OntologyIRI, or NULL if not found.
   */
   Doc_id const* find_doc_iri(std::string const& iri) const {
      T const& self = static_cast<T const&>(*this);
      Node_id const* nid = self.find_iri_node(iri);
      if( ! nid ) return 0;
      const Doc_map::version_range vr = docs_.find_version(*nid);
      if( vr ) return &vr.front();
      const Doc_map::iri_range ir = docs_.find_iri(*nid);
      if( ir ) return &ir.front();
      return 0;
   }

   /**
    @param path
    @param iri
    @param vers
    @return
   */
   std::pair<Doc_id,bool> insert_doc(
            std::string const& path,
            std::string const& iri,
            std::string const& vers = ""
   ) {
      T& self = static_cast<T&>(*this);
      const Node_id iid = self.insert_iri_node(iri);
      const Node_id vid =
               vers.empty() ? terms::T_empty_::id() : self.insert_iri_node(vers);
      return insert_doc(path, iid, vid);
   }

   /**
    @param path
    @param iri
    @param vers
    @return
   */
   std::pair<Doc_id,bool> insert_doc(
            std::string const& path,
            const Node_id iri,
            const Node_id vers
   ) {
      return docs_.insert(path, iri, vers);
   }

   /**
    @param did document ID; invalid @b id results in undefined behavior
    @return node ID for ontology IRI
   */
   Node_id ontology_iri_id(const Doc_id did) const {return docs_.ontology_iri(did);}

   /**
    @param did document ID
    @return node ID for document version or empty node ID if version is not defined
   */
   Node_id version_iri_id(const Doc_id did) const {return docs_.version_iri(did);}

   /**
    @param did document ID
    @return document's path or empty string if not defined
   */
   std::string path(const Doc_id did) const {return docs_.path(did);}

   std::string ontology_iri(const Doc_id did) const {
      return static_cast<T const*>(this)->string(ontology_iri_id(did));
   }

   std::string version_iri(const Doc_id did) const {
      return static_cast<T const*>(this)->string(version_iri_id(did));
   }
protected:
   Doc_map& documents() {return docs_;}

private:
   doc_map_t docs_;
};

}//namespace owlcpp
#endif /* STORE_DOC_CRTPB_HPP_ */
