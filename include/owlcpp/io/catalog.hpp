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
#include "owlcpp/rdf/store_node_iri_crtpb.hpp"
#include "owlcpp/rdf/store_doc_crtpb.hpp"
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
public Store_doc_crtpb<Catalog>,
private Store_node_iri_crtpb<Catalog>
{
public:
   typedef Iri_map iri_map;
   typedef Node_map node_map;
   typedef Doc_map doc_map;
private:
   Iri_map& iris() {return iri_;}
   node_map& nodes() {return node_;}
   Iri_map const& iris() const {return iri_;}
   node_map const& nodes() const {return node_;}
   friend struct Store_node_iri_crtpb<Catalog>;
   friend class Store_doc_crtpb<Catalog>;

public:
   typedef Doc_map::iterator iterator;
   typedef Doc_map::const_iterator const_iterator;
   struct Err : public Input_err {};
   std::size_t size() const {return documents().size();}
   const_iterator begin() const {return documents().begin();}
   const_iterator end() const {return documents().end();}
   std::string ontology_iri_str(const Doc_id did) const {return string(ontology_iri(did));}
   std::string version_iri_str(const Doc_id did) const { return string(version_iri(did)); }

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
   node_map node_;
};

}//namespace owlcpp
#endif /* CATALOG_HPP_ */
