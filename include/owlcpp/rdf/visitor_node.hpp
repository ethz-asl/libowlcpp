/** @file "/owlcpp/include/owlcpp/rdf/visitor_node.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef VISITOR_NODE_HPP_
#define VISITOR_NODE_HPP_

namespace owlcpp{

class Node_iri;
class Node_blank;
class Node_literal;

/**@brief 
*******************************************************************************/
class Visitor_node {
public:
   void visit(Node_iri const& node) {visit_impl(node);}
   void visit(Node_blank const& node) {visit_impl(node);}
   void visit(Node_literal const& node) {visit_impl(node);}

   virtual ~Visitor_node() {}
private:
   virtual void visit_impl(Node_iri const&) = 0;
   virtual void visit_impl(Node_blank const&) = 0;
   virtual void visit_impl(Node_literal const&) = 0;
};

#define OWLCPP_VISITABLE \
   void accept_impl(Visitor_node& visitor) const {visitor.visit(*this);} \
/* */


}//namespace owlcpp
#endif /* VISITOR_NODE_HPP_ */
