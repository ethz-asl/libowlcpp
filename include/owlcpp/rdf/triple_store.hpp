/** @file "/owlcpp/include/owlcpp/rdf/triple_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_HPP_
#define TRIPLE_STORE_HPP_

#include "owlcpp/rdf/node_store_iri_base.hpp"
#include "owlcpp/rdf/node_store_aux_base.hpp"
#include "owlcpp/rdf/doc_store_base.hpp"
#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"
#include "owlcpp/rdf/owl_terms.hpp"
#include "owlcpp/rdf/copy_triples.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Triple_store :
public Node_store_iri_base<Triple_store>,
public Node_store_aux_base<Triple_store>,
public Doc_store_base<Triple_store>
{
   friend class Node_store_iri_base<Triple_store>;
   friend class Node_store_aux_base<Triple_store>;
   friend class Doc_store_base<Triple_store>;

public:
   struct Err : public base_exception {};
   typedef Iri_map iri_map_t;
   typedef Node_map node_map_t;
   typedef Doc_map doc_map_t;
   typedef Triple_map triple_map_t;

   Triple_store() {insert_owl_terms(node_);}

   Iri_map& iris() {return iri_;}
   Iri_map const& iris() const {return iri_;}
   Node_map& nodes() {return node_;}
   Node_map const& nodes() const {return node_;}
   Doc_map const& documents() const {return doc_;}
   Doc_map& documents() {return doc_;}
   Triple_map const& triples() const {return triple_;}

   void insert_triple(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      triple_.insert(Triple(subj, pred, obj, doc));
   }

   Node const& operator[](const Node_id nid) const {return node_[nid];}
   std::string operator[](const Ns_id iid) const {return iri_[iid];}

private:
   Iri_map iri_;
   Node_map node_;
   Doc_map doc_;
   Triple_map triple_;
};

}//namespace owlcpp
#endif /* TRIPLE_STORE_HPP_ */
