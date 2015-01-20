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
#include "owlcpp/io/exception.hpp"
#include "owlcpp/io/detail/map_traits.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/rdf/map_std_ns_crtpb.hpp"
#include "owlcpp/rdf/map_std_node_crtpb.hpp"
#include "owlcpp/rdf/nodes_std.hpp"
#include "owlcpp/rdf/map_doc.hpp"
#include "owlcpp/rdf/map_ns.hpp"
#include "owlcpp/rdf/map_node_iri.hpp"
#include "owlcpp/rdf/map_doc_crtpb.hpp"
#include "owlcpp/rdf/crtpb_ns_node_iri.hpp"

namespace owlcpp{

/**@brief Store locations, ontology IRIs, and version IRIs of OWL ontologies.
@details Locations should be unique; ontology IRIs may be repeated;
non-empty verions IRIs should be unique.
*******************************************************************************/
class OWLCPP_IO_DECL Catalog :
public Map_doc_crtpb<Catalog>,
public Map_std_ns_crtpb<Catalog>,
public Map_std_node_crtpb<Catalog>,
public Crtpb_ns_node_iri<Catalog>
{
   friend class Map_std_ns_crtpb<Catalog>;
   friend class Map_std_node_crtpb<Catalog>;
   friend class Map_doc_crtpb<Catalog>;

   typedef detail::Map_traits<Catalog> traits;
   typedef traits::map_std_type map_std_type;
   typedef traits::map_ns_type map_ns_type;
   typedef traits::map_node_type map_node_type;

public:
   typedef traits::map_doc_type map_doc_type;
   typedef map_doc_type::iterator iterator;
   typedef map_doc_type::const_iterator const_iterator;
   struct Err : public Input_err {};

   //bring in overloaded methods
   using Map_std_ns_crtpb<Catalog>::operator[];
   using Map_std_node_crtpb<Catalog>::operator[];
   using Map_doc_crtpb<Catalog>::operator[];
   using Map_std_ns_crtpb<Catalog>::at;
   using Map_std_node_crtpb<Catalog>::at;
   using Map_doc_crtpb<Catalog>::at;

   using Map_std_node_crtpb<Catalog>::insert_node_iri;
   using Crtpb_ns_node_iri<Catalog>::insert_node_iri;

   using Map_std_node_crtpb<Catalog>::find_node_iri;
   using Crtpb_ns_node_iri<Catalog>::find_node_iri;

   using Map_std_ns_crtpb<Catalog>::find;
   using Map_std_node_crtpb<Catalog>::find;
   using Map_doc_crtpb<Catalog>::find;

   Catalog() : map_std_(map_std_type::get(Nodes_none())) {}

   std::size_t size() const {return map_doc_.size();}
   const_iterator begin() const {return map_doc_.begin();}
   const_iterator end() const {return map_doc_.end();}

   map_ns_type const& map_ns() const {return map_ns_;}
   map_node_type const& map_node() const {return map_node_;}
   map_doc_type const& map_doc() const {return map_doc_;}

   Node_id ontology_iri_id(const Doc_id did) const {
      return map_doc_[did].ontology_iri;
   }

   Node_id version_iri_id(const Doc_id did) const {
      return map_doc_[did].version_iri;
   }

   std::string path(const Doc_id did) const {return map_doc_[did].path;}
   std::string ontology_iri_str(const Doc_id) const;
   std::string version_iri_str(const Doc_id) const;

private:
   map_std_type const& map_std_;
   map_ns_type map_ns_;
   map_node_type map_node_;
   map_doc_type map_doc_;
};

/**Example of using Catalog
@example find_ontologies.cpp
*/

/**@brief determine OntologyIRI and VersionIRI of ontology document(s)
 and add them to the catalog
 @param cat catalog;
 @param path symbolic path pointing to local file or directory;
 any type implicitly convertible to boost::filesystem::path can be used,
 e.g., std::string, const char*
 @param recurse if true add to catalog files located in sub-directories
 @param search_depth once ontologyIRI declaration is found, stop searching for
 versionIRI declaration after @b search_depth triples
 @return number of added files
 @details
 If path is a directory, an attempt is made to parse every file located in it.
 Files that fail to parse are ignored.
*******************************************************************************/
OWLCPP_IO_DECL std::size_t add(
         Catalog& cat,
         boost::filesystem::path const& path,
         const bool recurse = false,
         const std::size_t search_depth = std::numeric_limits<std::size_t>::max()
);



}//namespace owlcpp
#endif /* CATALOG_HPP_ */
