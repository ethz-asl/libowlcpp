/** @file "/owl_cpp/include/owl_cpp/node_id.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
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
