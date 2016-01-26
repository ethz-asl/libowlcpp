/** @file "/owlcpp/lib/logic/factpp/data_instance.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "data_instance.hpp"
#include "fact++/Kernel.h"
#include "owlcpp/rdf/node_literal.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
D_value::D_value(Node_literal const& val, Triple_store const& ts)
: dt_(make_expression<Data_type>(val.datatype(), ts)),
  val_(val.value_str())
{}

/*
*******************************************************************************/
D_value::generated_t D_value::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   Expression<Data_type>::generated_t dt = dt_->get(k);
   return em.DataValue(val_, dt);
}


}//namespace factpp
}//namespace logic
}//namespace owlcpp
