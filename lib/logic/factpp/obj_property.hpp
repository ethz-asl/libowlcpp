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
   TDLObjectRoleExpression* get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRoleTop();
   }
};

/**@brief
*******************************************************************************/
struct Op_bottom : public Expression<Obj_prop> {
   TDLObjectRoleExpression* get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRoleBottom();
   }
};

/**@brief
*******************************************************************************/
class Op_declared : public Expression<Obj_prop> {
public:
   Op_declared(Expression_args const& ea, Triple_store const& ts)
   : iri_(ts.string(ea.handle)) {
      BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
   }

   TDLObjectRoleExpression* get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRole(iri_);
   }

private:
   std::string iri_;
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* OBJ_PROPERTY_HPP_ */
