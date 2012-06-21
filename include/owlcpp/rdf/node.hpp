/** @file "/owlcpp/include/owlcpp/rdf/node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RDF_NODE_HPP_
#define RDF_NODE_HPP_
#include <string>
#include <iosfwd>
#include "owlcpp/node_id.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/terms/iri_tags.hpp"

namespace owlcpp{

/**@brief Pure virtual interface for RDF nodes
*******************************************************************************/
class Node_v {
public:
   /**
    @return value string of the node
    @cond DEV
    @decision return by std::string value.\n
    Alternatives:
    - std::string const&: assumes existence of long-term std::string object;
    not every kind of node will have this.
    - std::string: may return newly generated object, which maybe slower
    @endcond
    */
   std::string value_str() const {return value_str_impl();}

   std::ostream& value_print(std::ostream& os) const {return print_impl(os);}

   Ns_id ns_id() const {return ns_id_impl();}

   bool empty() const {return empty_impl();}

   bool operator== (const Node_v& n) const {return equal_impl(n);}
   bool operator!= (const Node_v& n) const {return ! equal_impl(n);}

   std::size_t hash() const {return hash_impl();}

   virtual ~Node_v() {}
private:
   virtual std::string value_str_impl() const =0;

   virtual std::ostream& print_impl(std::ostream&) const =0;

   virtual Ns_id ns_id_impl() const =0;

   virtual bool empty_impl() const =0;

   virtual bool equal_impl(const Node_v& n) const =0;

   virtual std::size_t hash_impl() const =0;
};

}//namespace owlcpp
#endif /* RDF_NODE_HPP_ */
