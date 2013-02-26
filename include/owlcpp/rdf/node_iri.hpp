/** @file "/owlcpp/include/owlcpp/rdf/node_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_IRI_HPP_
#define NODE_IRI_HPP_
#include "boost/functional/hash.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/terms/ns_iri_tags.hpp"
#include "owlcpp/rdf/visitor_node.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_iri : public Node {
   friend inline std::size_t hash_value(Node_iri const& node)
   {return node.hash_impl();}

public:

   explicit Node_iri(const Ns_id ns = terms::empty::id(), std::string const& val = "")
   : val_(val), ns_(ns)
   {
      if( is_blank(ns_) ) BOOST_THROW_EXCEPTION(
               Rdf_err() << Rdf_err::msg_t("blank \"_\" namespace in IRI node")
      );
   }

   std::string const& fragment() const {return val_;}

private:
   std::string val_;
   Ns_id ns_;

   OWLCPP_VISITABLE

   Ns_id ns_id_impl() const { return ns_; }

   bool equal_impl(const Node& n) const {
      //todo: use typeid()?
      if( Node_iri const*const p = dynamic_cast<Node_iri const*>(&n) ) {
         return ns_ == p->ns_ && val_ == p->val_;
      }
      return false;
   }

   std::size_t hash_impl() const {
      std::size_t h = 0;
      boost::hash_combine(h, val_);
      boost::hash_combine(h, ns_);
      return h;
   }

   Node* clone_impl() const {return new Node_iri(*this);}
};

}//namespace owlcpp
#endif /* NODE_IRI_HPP_ */
