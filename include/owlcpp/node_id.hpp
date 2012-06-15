/** @file "/owlcpp/include/owlcpp/node_id.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef NODE_ID_HPP_
#define NODE_ID_HPP_
#include "owlcpp/detail/object_id_base.hpp"

namespace owlcpp{

/**@brief Term identifier
*******************************************************************************/
class Node_id : public detail::Base_id<Node_id, unsigned> {
public:
   explicit Node_id(const unsigned x) : base(x) {}
   Node_id() : base(0) {}
};

}//namespace owlcpp
#endif /* NODE_ID_HPP_ */
