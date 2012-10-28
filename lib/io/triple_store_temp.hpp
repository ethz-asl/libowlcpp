/** @file "/owlcpp/lib/io/triple_store_temp.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_TEMP_HPP_
#define TRIPLE_STORE_TEMP_HPP_
#include "owlcpp/terms/node_tags_system.hpp"
#include "io/map_traits.hpp"
#include "owlcpp/rdf/map_ns.hpp"
#include "owlcpp/rdf/map_node.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/rdf/map_std_ns_crtpb.hpp"
#include "owlcpp/rdf/map_std_node_crtpb.hpp"
#include "owlcpp/rdf/doc_meta.hpp"
#include "owlcpp/rdf/nodes_std.hpp"
#include "owlcpp/rdf/map_node_literal_crtpb.hpp"
#include "owlcpp/rdf/map_triple.hpp"
#include "owlcpp/rdf/map_triple_crtpb.hpp"
#include "owlcpp/rdf/crtpb_ns_node_iri.hpp"

namespace owlcpp{ namespace detail{

/**@brief Storage for RDF triples coming from a single document
*******************************************************************************/
class Triple_store_temp :
public Map_std_ns_crtpb<Triple_store_temp>,
public Map_std_node_crtpb<Triple_store_temp>,
public Crtpb_ns_node_iri<Triple_store_temp>,
public Map_node_literal_crtpb<Triple_store_temp>,
public Map_triple_crtpb<Triple_store_temp>
{
   friend class Map_std_ns_crtpb<Triple_store_temp>;
   friend class Map_std_node_crtpb<Triple_store_temp>;
   friend class Map_node_literal_crtpb<Triple_store_temp>;
   friend class Map_triple_crtpb<Triple_store_temp>;

   typedef Map_traits<Triple_store_temp> traits;
   typedef traits::map_std_type map_std_type;

public:
   typedef traits::map_ns_type map_ns_type;
   typedef traits::doc_type doc_type;
   typedef traits::map_node_type map_node_type;
   typedef traits::map_triple_type map_triple_type;

   struct Err : public base_exception {};

   Triple_store_temp(map_std_type const& map_std, std::string const& path)
   : map_std_(map_std),
     doc_(terms::T_empty_::id(), terms::T_empty_::id(), path),
     did_(0)
   {}

   map_std_type const& map_std() const {return map_std_;}
   map_ns_type const& map_ns() const {return map_ns_;}
   map_node_type const& map_node() const {return map_node_;}
   map_triple_type const& map_triple() const {return map_triple_;}

   Node_id insert_blank(const unsigned n) {
      return map_node_.insert_blank(n, did_);
   }

   void insert_triple(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj
   ) {
      map_triple_.insert(subj, pred, obj, did_);
   }

   doc_type const& operator[](const Doc_id) const {return doc_;}
   std::string const& path() const {return doc_.path;}

   void set_ids(std::string const& ontologyIRI, std::string const& versionIRI) {
      doc_.ontology_iri = insert_node_iri(ontologyIRI);
      doc_.version_iri = insert_node_iri(versionIRI);
   }

   void clear() {
      map_triple_.clear();
      map_node_.clear();
      map_ns_.clear();
   }

   //disambiguate overloaded methods
   using Map_std_ns_crtpb<Triple_store_temp>::operator[];
   using Map_std_node_crtpb<Triple_store_temp>::operator[];
   using Map_std_ns_crtpb<Triple_store_temp>::at;
   using Map_std_node_crtpb<Triple_store_temp>::at;

   using Map_std_node_crtpb<Triple_store_temp>::insert_node_iri;
   using Crtpb_ns_node_iri<Triple_store_temp>::insert_node_iri;

   using Map_std_node_crtpb<Triple_store_temp>::find_node_iri;
   using Crtpb_ns_node_iri<Triple_store_temp>::find_node_iri;

   using Map_std_ns_crtpb<Triple_store_temp>::valid;
   using Map_std_node_crtpb<Triple_store_temp>::valid;

private:
   map_std_type const& map_std_;
   map_ns_type map_ns_;
   map_node_type map_node_;
   map_triple_type map_triple_;
   doc_type doc_;
   const Doc_id did_;
};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_STORE_TEMP_HPP_ */
