/** @file "/owlcpp/include/owlcpp/rdf/node_blank.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_BLANK_HPP_
#define NODE_BLANK_HPP_
#include "boost/functional/hash.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/rdf/visitor_node.hpp"
#include "owlcpp/terms/ns_iri_tags.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_blank : public Node {
public:
   Node_blank(const unsigned n, const Doc_id doc)
   : n_(n), doc_(doc) {}

   unsigned index() const {return n_;}
   Doc_id document() const {return doc_;}

private:
   unsigned n_;
   Doc_id doc_;

   OWLCPP_VISITABLE

   Ns_id ns_id_impl() const { return terms::blank::id(); }

   bool equal_impl(const Node& n) const {
      //todo: use typeid()?
      if( Node_blank const*const p = dynamic_cast<Node_blank const*>(&n) ) {
         return n_ == p->n_ && doc_ == p->doc_;
      }
      return false;
   }

   std::size_t hash_impl() const {
      std::size_t h = 0;
      boost::hash_combine(h, n_);
      boost::hash_combine(h, doc_);
      return h;
   }

   Node* clone_impl() const {return new Node_blank(*this);}

};

}//namespace owlcpp
#endif /* NODE_BLANK_HPP_ */
