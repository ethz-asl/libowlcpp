/** @file "/owlcpp/include/owlcpp/rdf/print_triple.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef PRINT_TRIPLE_HPP_
#define PRINT_TRIPLE_HPP_
#include <sstream>
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
inline std::string to_string(Triple const& t) {
   std::ostringstream str;
   str
   << to_string(t.subj_) << ' '
   << to_string(t.pred_) << ' '
   << to_string(t.obj_)
   ;
   return str.str();
}

/**@brief
*******************************************************************************/
inline std::string to_string(Triple const& t, Triple_store const& ts) {
   std::ostringstream str;
   str
   << to_string(t.subj_, ts) << ' '
   << to_string(t.pred_, ts) << ' '
   << to_string(t.obj_, ts)
   ;
   return str.str();
}

}//namespace owlcpp
#endif /* PRINT_TRIPLE_HPP_ */
