/** @file "/owlcpp/include/owlcpp/node_base.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#ifndef NODE_BASE_HPP_
#define NODE_BASE_HPP_
#include <string>
#include <memory>
#include <vector>
#include "boost/range.hpp"
#include "boost/functional/hash.hpp"
namespace b = boost;

#include "loki/Visitor.h"
namespace L = Loki;

#include "owlcpp/ns_id.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{

class Triple;
class Node_id;

/** Interface for a term of an RDF triple
*******************************************************************************/
struct Node_base : public L::BaseVisitable<void, L::DefaultCatchAll, true> {
   typedef Node_id id_t;
   struct exception : public base_exception {};

   bool operator<(const Node_base& t) const {
      if( ns_id() < t.ns_id() ) return true;
      if( ns_id() > t.ns_id() ) return false;
      return value_str() < t.value_str();
   }

   bool operator== (const Node_base& t) const {
      return ns_id() == t.ns_id() && value_str() == t.value_str();
   }

   virtual Ns_id ns_id() const = 0;

   /** Make a new triple
    * @param subj subject node ID
    * @param pred predicate node ID
    * @param obj object node ID
    * @return pointer to RDF triple of appropriate type; auto_ptr is returned
    * to indicate that the triple was created on the heap and the ownership
    * is transfered to the caller
    */
   virtual std::auto_ptr<Triple> make_triple(
         const Node_id subj, const Node_id pred, const Node_id obj) const = 0;

   virtual const std::string& value_str() const  = 0;
   virtual ~Node_base() {}
   virtual ReturnType Accept(L::BaseVisitor&) const = 0;
};

/**
*******************************************************************************/
inline std::size_t hash_value(Node_base const& node) {
    std::size_t seed = 0;
    b::hash_combine(seed, node.ns_id()());
    b::hash_combine(seed, node.value_str());
    return seed;
}

}//namespace owlcpp
#endif /* NODE_BASE_HPP_ */
