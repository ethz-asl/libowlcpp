/** @file "/owlcpp/include/owlcpp/rdf/print_ns.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PRINT_NS_HPP_
#define PRINT_NS_HPP_
#include <string>
#include "owlcpp/ns_id.hpp"

namespace owlcpp{

/**@return namespace ID string
*******************************************************************************/
inline std::string to_string(const Ns_id nsid) {
   return "NsID" + boost::lexical_cast<std::string>(nsid());
}


}//namespace owlcpp
#endif /* PRINT_NS_HPP_ */
