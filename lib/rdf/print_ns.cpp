/** @file "/owlcpp/lib/rdf/print_ns.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/print_ns.hpp"

#include "boost/lexical_cast.hpp"

namespace owlcpp {

/**@return namespace ID string
*******************************************************************************/
std::string to_string(const Ns_id nsid) {
   return "Ns" + boost::lexical_cast<std::string>(nsid());
}


}//namespace owlcpp
