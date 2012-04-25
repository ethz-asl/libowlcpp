/** @file "/owlcpp/include/owlcpp/rdf/triple_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_HPP_
#define TRIPLE_STORE_HPP_

#include "owlcpp/rdf/store_node_iri_crtpb.hpp"
#include "owlcpp/rdf/store_node_blank_crtpb.hpp"
#include "owlcpp/rdf/store_node_literal_crtpb.hpp"
#include "owlcpp/rdf/store_doc_crtpb.hpp"
#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "owlcpp/rdf/copy_triples.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Triple_store :
public Store_node_iri_crtpb<Triple_store>,
public Store_node_blank_crtpb<Triple_store>,
public Store_node_literal_crtpb<Triple_store>,
public Store_doc_crtpb<Triple_store>
{
/*
   friend class Store_node_iri_crtpb<Triple_store>;
   friend class Store_node_literal_crtpb<Triple_store>;
   friend class Store_node_blank_crtpb<Triple_store>;
   friend class Store_doc_crtpb<Triple_store>;
*/

public:
   struct Err : public Rdf_err {};
   typedef Iri_map iri_map;
   typedef Node_map node_map;
   typedef Triple_map triple_map;

   Triple_store(Node_map_std const& snodes = Node_map_std::get(Nodes_owl()))
   : iri_(snodes), node_(snodes), triple_()
   {}

   Iri_map& iris() {return iri_;}
   Iri_map const& iris() const {return iri_;}
   node_map& nodes() {return node_;}
   node_map const& nodes() const {return node_;}
   Triple_map const& triples() const {return triple_;}

   void insert_triple(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      triple_.insert(subj, pred, obj, doc);
   }

   void clear() {
      triple_.clear();
      documents().clear();
      node_.clear();
      iri_.clear();
   }

private:
   Iri_map iri_;
   node_map node_;
   Triple_map triple_;
};

}//namespace owlcpp
#endif /* TRIPLE_STORE_HPP_ */
