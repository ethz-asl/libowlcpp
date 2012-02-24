/** @file "/owlcpp/lib/io/triple_store_temp.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_TEMP_HPP_
#define TRIPLE_STORE_TEMP_HPP_
#include "owlcpp/rdf/store_node_iri_crtpb.hpp"
#include "owlcpp/rdf/store_node_blank_crtpb.hpp"
#include "owlcpp/rdf/store_node_literal_crtpb.hpp"
#include "owlcpp/rdf/store_doc_crtpb.hpp"
#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace detail{

/**@brief Storage for RDF triples coming from a single document
*******************************************************************************/
class Triple_store_temp :
public Store_node_iri_crtpb<Triple_store>,
public Store_node_blank_crtpb<Triple_store>,
public Store_node_literal_crtpb<Triple_store>
{
public:

   typedef Node_map node_map_t;
   struct Err : public base_exception {};
   Iri_map& iris() {return iri_;}
   Iri_map const& iris() const {return iri_;}
   node_map_t& nodes() {return node_;}
   node_map_t const& nodes() const {return node_;}
   Triple_map const& triples() const {return triple_;}

   void insert_triple(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj
   ) {
      triple_.insert(Triple(subj, pred, obj, did_));
   }

   Node_id ontology_iri(const Doc_id = Doc_id(0)) const {return ontology_iri_;}
   Node_id version_iri(const Doc_id) const {return version_iri_;}
   std::string path(const Doc_id) const {return path_;}


private:
   const Doc_id did_;
   Node_id ontology_iri_;
   Node_id version_iri_;
   std::string path_;
   Iri_map iri_;
   node_map_t node_;
   Triple_map triple_;
};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_STORE_TEMP_HPP_ */
