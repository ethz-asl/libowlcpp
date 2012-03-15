/** @file "/owlcpp/lib/logic/expressions_op.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXPRESSIONS_OP_HPP_
#define EXPRESSIONS_OP_HPP_
#include "expressions.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/**@brief
*******************************************************************************/
class Op_declared : public Obj_prop {
public:
   Op_declared(const Node_id nid) : nid_(nid) {
      if( is_empty(nid_) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("empty object property node")
               << Err::str1_t(ts.string(card_type_))
      );
   }

   TDLObjectRoleExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      return k.getExpressionManager()->ObjectRole(ts.string(nid_));
   }

private:
   const Node_id nid_;
};

}//namespace detail
}//namespace owlcpp
#endif /* EXPRESSIONS_OP_HPP_ */
