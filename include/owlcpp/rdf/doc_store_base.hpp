/** @file "/owlcpp/include/owlcpp/rdf/doc_store_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_STORE_BASE_HPP_
#define DOC_STORE_BASE_HPP_
#include "owlcpp/rdf/doc_info_map.hpp"

namespace owlcpp{

/**@brief CRTP base for document info stores
*******************************************************************************/
template<class T> struct Doc_store_base {

   /**
    @param iri OntologyIRI or VersionIRI
    @return pointer to document ID for the first document that has specified VersionIRI or,
    if not found, for the first document that has specified OntologyIRI, or NULL if not found.
   */
   Doc_id const* find_doc_iri(std::string const& iri) const {
      T const& self = static_cast<T const&>(*this);
      Node_id const* nid = self.find_iri_node(iri);
      if( ! nid ) return 0;
      const Doc_map::version_range vr = self.documents().find_version(*nid);
      if( vr ) return &vr.front();
      const Doc_map::iri_range ir = self.documents().find_iri(*nid);
      if( ir ) return &ir.front();
      return 0;
   }

   /**
    @param path
    @param iri
    @param version
    @return
   */
   std::pair<Doc_id,bool> insert_doc(
            std::string const& path,
            std::string const& iri,
            std::string const& version = std::string()
   ) {
      T& self = static_cast<T&>(*this);
      const Node_id iid = self.insert_iri_node(iri);
      if( version.empty() ) return self.documents().insert(path, iid);
      const Node_id vid = self.insert_iri_node(version);
      return self.documents().insert(path, iid, vid);
   }

};

}//namespace owlcpp
#endif /* DOC_STORE_BASE_HPP_ */
