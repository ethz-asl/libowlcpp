/** @file "/owlcpp/include/owlcpp/node.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_HPP_
#define NODE_HPP_
#include <string>
#include <iosfwd>
#include "boost/functional/hash.hpp"
#include "owlcpp/terms/iri_tags.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
*******************************************************************************/
class Node {
   Node();
public:
   /** make Node from standard node tag */
   template<class Tag> explicit Node(Tag const&)
   : val_(Tag::name()), ns_(Tag::ns_type::id()) {}

   Node(const Ns_id ns, std::string const& val)
   : val_(val), ns_(ns) {}

   std::string value_str() const {return val_;}
   Ns_id ns_id() const {return ns_;}

   bool operator<(const Node& n) const {
      if( ns_id() < n.ns_id() ) return true;
      if( ns_id() > n.ns_id() ) return false;
      return value_str() < n.value_str();
   }

   bool operator== (const Node& n) const {
      return ns_id() == n.ns_id() && value_str() == n.value_str();
   }

   std::size_t hash() const {
      std::size_t x = 0;
      boost::hash_combine(x, ns_());
      boost::hash_combine(x, boost::hash_value(val_));
      return x;
   }

   bool empty() const {return is_empty(ns_) && val_.empty();}

private:
   const std::string val_;
   const Ns_id ns_;
};

/**
*******************************************************************************/
template<class ChT, class Tr> inline std::basic_ostream<ChT,Tr>& operator<<(
      std::basic_ostream<ChT,Tr>& os, Node const& n) {
   return os << "ns" << n.ns_id()() << ':' << n.value_str();
}

/**
*******************************************************************************/
inline std::size_t hash_value(Node const& n) { return n.hash(); }


}//namespace owlcpp
#endif /* NODE_HPP_ */
