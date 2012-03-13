/** @file "/owlcpp/lib/reasoner/expressions.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXPRESSIONS_HPP_
#define EXPRESSIONS_HPP_
#include <vector>
#include "boost/shared_ptr.hpp"

#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/reasoner/exception.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Obj_type{
   Obj_type();
public:
   typedef boost::shared_ptr<const Obj_type> ptr_t;
   struct Err : public Reasoner_err {};

   Obj_type(const Node_id h, Triple_store const& ts) : ts_(ts), h_(h) {}

   virtual TDLConceptExpression* operator()(ReasoningKernel& k) const {
      return k.getExpressionManager()->Concept(ts_.string(h_));
   }

protected:
   Triple_store const& ts_;
   Node_id h_;
};

/**@brief
*******************************************************************************/
class Ot_type_list : public Obj_type {
   Ot_type_list();
public:
   struct Err : public Reasoner_err {};

   Ot_type_list(const Node_id handle, Triple_store const& ts);

   TDLConceptExpression* operator()(ReasoningKernel& k) const;

private:
   Triple_store const& ts_;
   Node_id handle_;
   Node_id type_;
   Node_id op_;
   std::vector<Ot_type_list> tv_;
   std::vector<Node_id> iv_;


   TDLConceptExpression* generate_expression(ReasoningKernel& k) const;
   void parse_expression();
   void parse_class();
   void parse_restriction();
};

/**
*******************************************************************************/
Obj_type::ptr_t make_obj_type(const Node_id h, Triple_store const& ts);

}//namespace detail
}//namespace owlcpp
#endif /* EXPRESSIONS_HPP_ */
