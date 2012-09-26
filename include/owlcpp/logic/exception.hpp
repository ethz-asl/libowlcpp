/** @file "/owlcpp/include/owlcpp/logic/exception.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef REASONER_EXCEPTION_HPP_
#define REASONER_EXCEPTION_HPP_
#include "owlcpp/exception.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
struct Logic_err : public base_exception {
   typedef boost::error_info<struct errinfo_node_id_, Node_id> node_id_t;
};

}//namespace owlcpp
#endif /* REASONER_EXCEPTION_HPP_ */
