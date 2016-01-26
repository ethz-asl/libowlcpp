/** @file "/owlcpp/lib/logic/factpp/data_property.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "data_property.hpp"
#include "fact++/Kernel.h"
#include "boost/assert.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{

/*
*******************************************************************************/
Dp_top::generated_t Dp_top::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->DataRoleTop();
}

/*
*******************************************************************************/
Dp_bottom::generated_t Dp_bottom::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->DataRoleBottom();
}

/*
*******************************************************************************/
Dp_declared::Dp_declared(Expression_args const& ea, Triple_store const& ts)
: iri_(to_string(ea.handle, ts)) {
   BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
}

/*
*******************************************************************************/
Dp_declared::generated_t Dp_declared::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->DataRole(iri_);
}


}//namespace factpp
}//namespace logic
}//namespace owlcpp
