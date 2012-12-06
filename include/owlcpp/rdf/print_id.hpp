/** @file "/owlcpp/include/owlcpp/rdf/print_id.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PRINT_ID_HPP_
#define PRINT_ID_HPP_
#include <string>
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{
class Ns_id;
class Node_id;
class Doc_id;

/**@return namespace ID string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(const Ns_id);

/**@return namespace ID string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(const Node_id);

/**@return namespace ID string
*******************************************************************************/
OWLCPP_RDF_DECL std::string to_string(const Doc_id);

}//namespace owlcpp
#endif /* PRINT_ID_HPP_ */
