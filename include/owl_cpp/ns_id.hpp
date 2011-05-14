/** @file "/owl_cpp/include/owl_cpp/ns_id.hpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef NS_ID_HPP_
#define NS_ID_HPP_
#include "owl_cpp/comparable_value.hpp"

namespace owl_cpp{

/** Namespace identifier
*******************************************************************************/
class Ns_id : public unsigned_val_t {
public:
   Ns_id() : unsigned_val_t(0) {}
   explicit Ns_id(const unsigned x) : unsigned_val_t(x) {}
};


}//namespace owl_cpp

#endif /* NS_ID_HPP_ */
