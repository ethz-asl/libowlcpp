/** @file "/owlcpp/lib/logic/factpp/obj_property.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OBJ_PROPERTY_HPP_
#define OBJ_PROPERTY_HPP_
#include "expression.hpp"
#include "factpp/Kernel.hpp"
#include "boost/assert.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/**@brief
*******************************************************************************/
struct Op_top : public Expression<Obj_prop> {
   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRoleTop();
   }
};

/**@brief
*******************************************************************************/
struct Op_bottom : public Expression<Obj_prop> {
   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRoleBottom();
   }
};

/**@brief
*******************************************************************************/
class Op_declared : public Expression<Obj_prop> {
public:
   Op_declared(Expression_args const& ea, Triple_store const& ts)
   : iri_(to_string(ea.handle, ts)) {
      BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
   }

   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRole(iri_);
   }

private:
   std::string iri_;
};

/**@brief
*******************************************************************************/
class Op_inverse : public Expression<Obj_prop> {
public:
   Op_inverse(Expression_args const& ea, Triple_store const& ts)
   : op_(make_expression<Obj_prop>(ea.obj1, ts)) {
      if( ! is_iri(ts[ea.handle].ns_id()) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("non-IRI object in _:x owl:inverseOf *:y")
               << Err::str1_t(to_string_short(ea.obj1, ts))
      );
   }

   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Inverse(op_->get(k));
   }

private:
   ptr_t op_;
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* OBJ_PROPERTY_HPP_ */
