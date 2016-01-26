/** @file "/owlcpp/lib/logic/factpp/data_type.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DATA_TYPE_HPP_
#define DATA_TYPE_HPP_
#include <string>
#include "expression.hpp"
#include "fact++/tDLExpression.h"

namespace owlcpp{ namespace logic{ namespace factpp{


/**@brief
*******************************************************************************/
class Dt_standard : public Expression<Data_type> {
public:
   Dt_standard(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   const Node_id nid_;
};

/**@brief
*******************************************************************************/
class Dt_declared : public Expression<Data_type>, public Expression<Data_range> {
public:
   Dt_declared(Expression_args const& ea, Triple_store const& ts);

   TDLDataTypeName* get(ReasoningKernel& k ) const;

private:
   std::string iri_;
};


}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* DATA_TYPE_HPP_ */
