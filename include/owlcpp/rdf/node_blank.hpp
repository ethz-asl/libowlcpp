/** @file "/owlcpp/include/owlcpp/rdf/node_blank.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_BLANK_HPP_
#define NODE_BLANK_HPP_
#include <iostream>
#include <sstream>
#include "boost/functional/hash.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/doc_id.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_blank : public Node {
public:
   Node_blank(const unsigned n, const Doc_id doc)
   : n_(n), doc_(doc) {}

private:
   unsigned n_;
   Doc_id doc_;

   std::string value_str_impl() const {
      std::stringstream ss;
      print_impl(ss);
      return ss.str();
   }

   std::ostream& print_impl(std::ostream& os) const {
      return os << 'n' << doc_ << '_' << n_;
   }

   Ns_id ns_id_impl() const { return terms::N_blank::id(); }

   bool empty_impl() const { return false; }

   bool equal_impl(const Node& n) const {
      //todo: use typeid()?
      if( Node_blank const*const p = dynamic_cast<Node_blank const*>(&n) ) {
         return n_ == p->n_ && doc_ == p->doc_;
      }
      return false;
   }

   std::size_t hash_impl() const {
      std::size_t x = n_;
      boost::hash_combine(x, hash_value(doc_));
      return x;
   }

};

}//namespace owlcpp
#endif /* NODE_BLANK_HPP_ */
