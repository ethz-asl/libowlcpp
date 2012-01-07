/** @file "/owlcpp/include/owlcpp/node_id.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef NODE_ID_HPP_
#define NODE_ID_HPP_
#include "owlcpp/detail/comparable_value.hpp"

namespace owlcpp{

/**@brief Term identifier
*******************************************************************************/
class Node_id : public detail::unsigned_val_t {
public:
   explicit Node_id(const unsigned x) : detail::unsigned_val_t(x) {}
   Node_id() : detail::unsigned_val_t(0) {}
};

}//namespace owlcpp
#endif /* NODE_ID_HPP_ */
