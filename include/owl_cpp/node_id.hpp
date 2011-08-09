/** @file "/owl_cpp/include/owl_cpp/node_id.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef NODE_ID_HPP_
#define NODE_ID_HPP_
#include "owl_cpp/comparable_value.hpp"

namespace owl_cpp{

/** Term identifier
*******************************************************************************/
class Node_id : public unsigned_val_t {
public:
   explicit Node_id(const unsigned x) : unsigned_val_t(x) {}
   Node_id() : unsigned_val_t(0) {}
};

}//namespace owl_cpp

#endif /* NODE_ID_HPP_ */
