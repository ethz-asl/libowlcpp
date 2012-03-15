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
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/logic/exception.hpp"
#include "expression_args.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
struct Obj_type {
   struct Err : public Logic_err {};

   typedef boost::shared_ptr<const Obj_type> ptr_t;

   static ptr_t make(const Node_id h, Triple_store const& ts) {
      return make(Expression_args(h, ts), ts);
   }

   static ptr_t make(Expression_args const& ea, Triple_store const& ts);

   virtual TDLConceptExpression* get(
            Triple_store const& ts,
            ReasoningKernel& k
   ) const  = 0;
};

/**@brief
*******************************************************************************/
struct Obj_prop {
   struct Err : public Logic_err {};

   typedef boost::shared_ptr<const Obj_prop> ptr_t;

   static ptr_t make(const Node_id h, Triple_store const& ts) {
      return make(Expression_args(h, ts), ts);
   }

   static ptr_t make(Expression_args const& ea, Triple_store const& ts);

   virtual TDLObjectRoleExpression* get(
            Triple_store const& ts,
            ReasoningKernel& k
   ) const  = 0;
};


/**@brief
*******************************************************************************/
class Ot_type_list : public Obj_type {
   Ot_type_list();
public:
   Ot_type_list(Expression_args const& ea, Triple_store const& ts);

   TDLConceptExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      return 0;
   }

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
