/** @file "/owlcpp/lib/logic/factpp/data_instance.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef DATA_INSTANCE_HPP_
#define DATA_INSTANCE_HPP_
#include "expression.hpp"

namespace owlcpp{
class Node_literal;

namespace logic{ namespace factpp{

/**@brief FacetRestriction
*******************************************************************************/
class D_value : public Expression<Data_inst> {
public:
   D_value(Node_literal const& val, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   Expression<Data_type>::ptr_t dt_;
   std::string val_;
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* DATA_INSTANCE_HPP_ */
