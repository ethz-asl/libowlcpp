/** @file "/owlcpp/lib/logic/expressions.hpp" 
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
#include "owlcpp/logic/exception.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
struct Obj_type {
   typedef boost::shared_ptr<const Obj_type> ptr_t;
   struct Err : public Logic_err {};

   virtual TDLConceptExpression* operator()(
            Triple_store const& ts,
            ReasoningKernel& k
   ) const  = 0;

   static ptr_t make(const Node_id h, Triple_store const& ts);
};

/**@brief
*******************************************************************************/
class Ot_declared : public Obj_type {
public:
   Ot_declared(const Node_id nid) : nid_(nid) {}

   TDLConceptExpression* operator()(Triple_store const& ts, ReasoningKernel& k ) const {
      return k.getExpressionManager()->Concept(ts.string(nid_));
   }

private:
   const Node_id nid_;
};

/**@brief
*******************************************************************************/
class Ot_complement : public Obj_type {
public:
   Ot_complement(const Node_id h, Triple_store const& ts)

private:
   ptr_t ot_;
};

/**@brief
*******************************************************************************/
class Ot_type_list : public Obj_type {
   Ot_type_list();
public:
   struct Err : public Logic_err {};

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

}//namespace detail
}//namespace owlcpp
#endif /* EXPRESSIONS_HPP_ */
