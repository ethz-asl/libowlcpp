/** @file "/owlcpp/include/owlcpp/rdf/hash_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef HASH_NODE_HPP_
#define HASH_NODE_HPP_
#include "boost/functional/hash.hpp"

#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"

namespace owlcpp{ namespace detail{
struct Hash_node : public Visitor_node {
   Hash_node() : hash_(0) {}
   std::size_t hash_;

   void visit_impl(Node_iri const& n) {
      boost::hash_combine(hash_, n.ns_id());
      boost::hash_combine(hash_, n.name());
   }

   void visit_impl(Node_blank const& n) {
      boost::hash_combine(hash_, n.index());
      boost::hash_combine(hash_, n.document());
   }

   void visit_impl(Node_literal const& n) {
      boost::hash_combine(hash_, n.datatype());
      boost::hash_combine(hash_, n.language());
      boost::hash_combine(hash_, n.value_str());
   }
};
}//namespace detail

/**
*******************************************************************************/
inline std::size_t hash_value(Node const& node) {
   detail::Hash_node hn;
   node.accept(hn);
   return hn.hash_;
}



}//namespace owlcpp
#endif /* HASH_NODE_HPP_ */
