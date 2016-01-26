/** @file "/owlcpp/lib/logic/factpp/obj_property.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "obj_property.hpp"
#include "fact++/Kernel.h"
#include "boost/assert.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{

/*
*******************************************************************************/
Op_top::generated_t Op_top::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->ObjectRoleTop();
}

/*
*******************************************************************************/
Op_bottom::generated_t Op_bottom::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->ObjectRoleBottom();
}

/*
*******************************************************************************/
Op_declared::Op_declared(Expression_args const& ea, Triple_store const& ts)
: iri_(to_string(ea.handle, ts)) {
   BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
}

/*
*******************************************************************************/
Op_declared::generated_t Op_declared::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->ObjectRole(iri_);
}

/*
*******************************************************************************/
Op_inverse::Op_inverse(Expression_args const& ea, Triple_store const& ts)
: op_(make_expression<Obj_prop>(ea.obj1, ts)) {
   if( ! is_iri(ts[ea.obj1].ns_id()) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("non-IRI object in _:x owl:inverseOf *:y")
            << Err::str1_t(to_string(ea.obj1, ts))
   );
}

/*
*******************************************************************************/
Op_inverse::generated_t Op_inverse::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->Inverse(op_->get(k));
}


}//namespace factpp
}//namespace logic
}//namespace owlcpp
