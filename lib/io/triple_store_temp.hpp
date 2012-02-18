/** @file "/owlcpp/lib/io/triple_store_temp.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_TEMP_HPP_
#define TRIPLE_STORE_TEMP_HPP_
#include "owlcpp/rdf/node_store_iri_base.hpp"
#include "owlcpp/rdf/node_store_aux_base.hpp"
#include "owlcpp/rdf/doc_store_base.hpp"
#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/node_map_owl.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace detail{

/**@brief Storage for RDF triples coming from a single document
*******************************************************************************/
class Triple_store_temp :
public Node_store_iri_base<Triple_store_temp>,
public Node_store_aux_base<Triple_store_temp>
{
public:

   typedef Node_map<Owl_nodes> node_map_t;
   struct Err : public base_exception {};
   Iri_map& iris() {return iri_;}
   Iri_map const& iris() const {return iri_;}
   node_map_t& nodes() {return node_;}
   node_map_t const& nodes() const {return node_;}
   Triple_map const& triples() const {return triple_;}

   void insert_triple(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      triple_.insert(Triple(subj, pred, obj, doc));
   }

private:
   Iri_map iri_;
   node_map_t node_;
   Triple_map triple_;
};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_STORE_TEMP_HPP_ */
