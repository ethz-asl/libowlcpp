/** @file "/owlcpp/include/owlcpp/rdf/node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RDF_NODE_HPP_
#define RDF_NODE_HPP_
#include "owlcpp/node_id.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/rdf/visitor_node.hpp"

namespace owlcpp{
/**@brief Pure virtual interface for RDF nodes
*******************************************************************************/
class Node {
public:
   Ns_id ns_id() const {return ns_id_impl();}
   bool empty() const {return empty_impl();}
   std::size_t hash() const {return hash_impl();}
   bool operator== (const Node& n) const {return equal_impl(n);}
   bool operator!= (const Node& n) const {return ! equal_impl(n);}
   void accept(Visitor_node& visitor) const {accept_impl(visitor);}
   virtual ~Node() {}

private:
   virtual Ns_id ns_id_impl() const =0;
   virtual bool empty_impl() const =0;
   virtual std::size_t hash_impl() const =0;
   virtual bool equal_impl(const Node& n) const =0;
   virtual void accept_impl(Visitor_node&) const = 0;
};


}//namespace owlcpp
#endif /* RDF_NODE_HPP_ */
