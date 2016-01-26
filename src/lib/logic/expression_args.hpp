/** @file "/owlcpp/lib/logic/expression_args.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXPRESSION_ARGS_HPP_
#define EXPRESSION_ARGS_HPP_
#include "owlcpp/node_id.hpp"
#include "owlcpp/logic/exception.hpp"

namespace owlcpp{
class Triple_store;
namespace logic{


/**@brief 
*******************************************************************************/
struct Expression_args {
   struct Err : public Logic_err {};

   Expression_args(const Node_id h, Triple_store const& ts);
   std::string string(Triple_store const& ts) const;

   Node_id handle, e_type, pred1, obj1, pred2, obj2, cardinality;
   unsigned num;
};


}//namespace logic
}//namespace owlcpp
#endif /* EXPRESSION_ARGS_HPP_ */
