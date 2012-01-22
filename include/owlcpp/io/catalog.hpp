/** @file "/owlcpp/include/owlcpp/io/catalog.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CATALOG_HPP_
#define CATALOG_HPP_
#include "boost/filesystem/path.hpp"

#include "owlcpp/io/config.hpp"
#include "owlcpp/rdf/iri_store.hpp"
#include "owlcpp/rdf/node_store.hpp"
#include "owlcpp/rdf/doc_info_store.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{

/**@brief Store locations, ontology IRIs, and version IRIs of OWL ontologies.
@details Locations should unique; ontology IRIs may be repeated;
non-empty verions IRIs should be unique.
*******************************************************************************/
class OWLCPP_IO_DECL Catalog {
public:
   struct Err : public base_exception {};

   /**
    @param path
    @param iri
    @param version
    @return
   */
   Doc_id insert_doc(
            std::string const& path,
            std::string const& iri,
            std::string const& version = std::string()
   );

   /**
    @param iri OntologyIRI or VersionIRI
    @return pointer to document ID for the first document that has specified VersionIRI or,
    if not found, for the first document that has specified OntologyIRI, or NULL if not found.
   */
   Doc_id const* find_doc(std::string const& iri) const;

   Node const& iri(const Doc_id did) const {return node_[doc_.iri(did)];}
   Node const* version(const Doc_id did) const;
   std::string path(const Doc_id did) const {return doc_.path(did);}

   /**@brief determine OntologyIRI and VersionIRI of ontology document(s)
    and add it to the catalog
    @param path symbolic path pointing to local file or directory;
    any type implicitly convertible to boost::path can be used, e.g., std::string, const char*
    @param recurse if true and path is a directory, recurse
    @return reference to self
    @throw Err
   */
   Catalog& add(boost::filesystem::path const& path, const bool recurse = false);

private:
   Iri_store iri_;
   Node_store node_;
   Doc_store doc_;

   /**@brief if not already present, store IRI reference node
    @param iri node IRI string;
    consistent uniform representation of non-ascii characters is assumed
    (e.g., UTF-8, or %HH)
    @return node ID
   */
   Node_id insert_iri_node(std::string const& iri);
};

   /**
   *******************************************************************************/
   std::pair<std::string,std::string> read_id(boost::filesystem::path const& path);

}//namespace owlcpp
#endif /* CATALOG_HPP_ */
