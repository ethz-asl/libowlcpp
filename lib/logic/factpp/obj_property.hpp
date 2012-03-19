/** @file "/owlcpp/lib/logic/factpp/obj_property.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OBJ_PROPERTY_HPP_
#define OBJ_PROPERTY_HPP_
#include <string>
#include "expression.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/**@brief
*******************************************************************************/
class Op_declared : public Fact_expression<Obj_prop> {
public:
   Op_declared(Expression_args const& ea, Triple_store const& ts)
   : iri_(ts.string(ea.handle)) {
      Node const& node = ts[ea.handle];
      if( is_blank(node.ns_id()) || is_empty(node.ns_id()) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid node for object property declaration")
               << Err::str1_t(ts.string(ea.handle))
      );
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
