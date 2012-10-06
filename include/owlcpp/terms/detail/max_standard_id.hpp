/** @file "/owlcpp/include/owlcpp/terms/detail/max_standard_id.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAX_STANDARD_ID_HPP_
#define MAX_STANDARD_ID_HPP_

#include "owlcpp/node_id.hpp"
#include "owlcpp/ns_id.hpp"

namespace owlcpp{ namespace detail{

inline Ns_id max_std_ns_id() {
   return Ns_id(
#ifdef OWLCPP_MAX_NUMBER_OF_STD_NAMESPACES
         OWLCPP_MAX_NUMBER_OF_STD_NAMESPACES
#else
         100
#endif
   );
}

inline Node_id max_std_node_id() {
   return Node_id(
#ifdef OWLCPP_MAX_NUMBER_OF_STD_TERMS
         OWLCPP_MAX_NUMBER_OF_STD_TERMS
#else
         1000
#endif
   );
}

}//namespace detail
}//namespace owlcpp
#endif /* MAX_STANDARD_ID_HPP_ */
