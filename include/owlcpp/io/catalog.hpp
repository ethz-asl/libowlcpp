/** @file "/owlcpp/include/owlcpp/io/catalog.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CATALOG_HPP_
#define CATALOG_HPP_
#include <limits>
#include "boost/filesystem/path.hpp"

#include "owlcpp/io/config.hpp"
#include "owlcpp/rdf/node_store_iri_base.hpp"
#include "owlcpp/rdf/doc_store_base.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"
#include "owlcpp/io/exception.hpp"

namespace owlcpp{

/**@brief Store locations, ontology IRIs, and version IRIs of OWL ontologies.
@details Locations should unique; ontology IRIs may be repeated;
non-empty verions IRIs should be unique.
*******************************************************************************/
class OWLCPP_IO_DECL Catalog :
public Doc_store_base<Catalog>,
private Node_store_iri_base<Catalog>
{
   Iri_map& iris() {return iri_;}
   Node_map& nodes() {return node_;}
   Doc_map& documents() {return doc_;}
   Iri_map const& iris() const {return iri_;}
   Node_map const& nodes() const {return node_;}
   Doc_map const& documents() const {return doc_;}
   friend class Node_store_iri_base<Catalog>;
   friend class Doc_store_base<Catalog>;

   /** indicate which namespaces should remain constant */
   static bool is_constant(const Ns_id ns) {
      switch ( ns() ) {
         case terms::N_blank::index:
         case terms::N_empty::index:
            return true;
         default:
            return false;
      }
   }

public:
   typedef Doc_map::id_iterator iterator;
   typedef iterator const_iterator;
   struct Err : public Input_err {};
   const_iterator begin() const {return doc_.begin();}
   const_iterator end() const {return doc_.end();}
   std::string iri(const Doc_id did) const {return string(iri_id(did));}
   std::string version(const Doc_id did) const { return string(version_id(did)); }
   std::string path(const Doc_id did) const {return doc_.path(did);}
   Node_id iri_id(const Doc_id did) const {return doc_.iri(did);}
   Node_id version_id(const Doc_id did) const {return doc_.version(did);}

   /**@brief determine OntologyIRI and VersionIRI of ontology document(s)
    and add them to the catalog
    @param path symbolic path pointing to local file or directory;
    any type implicitly convertible to boost::path can be used, e.g., std::string, const char*
    @param recurse if true add to catalog files located in sub-directories
    @param search_depth
    @return number of added files
    @details
    If path is a directory, an attempt will be made to parse and determine
    OntologyIRI and VersionIRI of every file located in it.
    The files that fail to parse will be ignored.
   */
   std::size_t add(
            boost::filesystem::path const& path,
            const bool recurse = false,
            const std::size_t search_depth = std::numeric_limits<std::size_t>::max()
   );

private:
   Iri_map iri_;
   Node_map node_;
   Doc_map doc_;
};

}//namespace owlcpp
#endif /* CATALOG_HPP_ */
