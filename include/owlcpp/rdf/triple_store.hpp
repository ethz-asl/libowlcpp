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
#include "owlcpp/rdf/node_store_owl.hpp"
#include "owlcpp/rdf/iri_map_owl.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class OWLCPP_RDF_DECL Triple_store :
public Node_store_iri_base<Triple_store>,
public Node_store_aux_base<Triple_store>,
public Doc_store_base<Triple_store>
{
   Iri_map_owl& iris() {return iri_;}
   Node_store_owl& nodes() {return node_;}
   Doc_map& documents() {return doc_;}
   friend class Node_store_iri_base<Triple_store>;
   friend class Node_store_aux_base<Triple_store>;
   friend class Doc_store_base<Triple_store>;

public:

   Iri_map_owl const& iris() const {return iri_;}
   Node_store_owl const& nodes() const {return node_;}
   Doc_map const& documents() const {return doc_;}
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
   Node const& iri(const Doc_id did) const {return node_[doc_.iri(did)];}
   std::string path(const Doc_id did) const {return doc_.path(did);}

   Node const* version(const Doc_id did) const {
      Node_id const* nid = doc_.version(did);
      if( nid ) return &node_[*nid];
      return 0;
   }

private:
   Iri_map_owl iri_;
   Node_store_owl node_;
   Doc_map doc_;
   Triple_map triple_;
};

}//namespace owlcpp
#endif /* TRIPLE_STORE_HPP_ */
