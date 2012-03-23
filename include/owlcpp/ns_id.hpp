/** @file "/owlcpp/include/owlcpp/ns_id.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef NS_ID_HPP_
#define NS_ID_HPP_
#include "owlcpp/detail/comparable_value.hpp"

namespace owlcpp{

/** Namespace identifier
*******************************************************************************/
class Ns_id : public detail::unsigned_val_t {
public:
   Ns_id() : detail::unsigned_val_t(0) {}
   explicit Ns_id(const unsigned x) : detail::unsigned_val_t(x) {}
};


}//namespace owlcpp

#endif /* NS_ID_HPP_ */
