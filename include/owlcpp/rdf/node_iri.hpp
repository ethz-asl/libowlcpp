/** @file "/owlcpp/include/owlcpp/rdf/node_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_IRI_HPP_
#define NODE_IRI_HPP_
#include <iostream>
#include "boost/functional/hash.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_iri : public Node {
public:
   static std::size_t make_hash(const Ns_id ns, std::string const& val) {
      std::size_t x = 0;
      boost::hash_combine(x, ns());
      boost::hash_combine(x, boost::hash_value(val));
      return x;
   }

   explicit Node_iri(const Ns_id ns = terms::N_empty::id(), std::string const& val = "")
   : val_(val), hash_(make_hash(ns, val)), ns_(ns)
   {
      if( is_blank(ns_) ) BOOST_THROW_EXCEPTION(
               Rdf_err() << Rdf_err::msg_t("blank \"_\" namespace in IRI node")
      );
   }

private:
   std::string val_;
   std::size_t hash_;
   Ns_id ns_;

   std::string value_str_impl() const {return val_;}

   std::ostream& print_impl(std::ostream& os) const {
      return os << val_;
   }

   Ns_id ns_id_impl() const { return ns_; }

   bool empty_impl() const { return is_empty(ns_) && val_.empty(); }

   bool equal_impl(const Node& n) const {
      //todo: use typeid()?
      if( Node_iri const*const p = dynamic_cast<Node_iri const*>(&n) ) {
         return ns_ == p->ns_ && val_ == p->val_;
      }
      return false;
   }

   std::size_t hash_impl() const { return hash_; }

};

}//namespace owlcpp
#endif /* NODE_IRI_HPP_ */
