/** @file "/owlcpp/lib/logic/factpp/data_property.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DATA_PROPERTY_HPP_
#define DATA_PROPERTY_HPP_
#include "expression.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{

/**@brief
*******************************************************************************/
struct Dp_top : public Expression<Data_prop> {
   generated_t get(ReasoningKernel& k ) const;
};

/**@brief
*******************************************************************************/
struct Dp_bottom : public Expression<Data_prop> {
   generated_t get(ReasoningKernel& k ) const;
};

/**@brief
*******************************************************************************/
class Dp_declared : public Expression<Data_prop> {
public:
   Dp_declared(Expression_args const& ea, Triple_store const& ts);
   generated_t get(ReasoningKernel& k ) const;

private:
   std::string iri_;
};


}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* DATA_PROPERTY_HPP_ */
