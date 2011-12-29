/** @file "/owlcpp/include/owlcpp/terms/uri_list.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef URI_LIST_HPP_
#define URI_LIST_HPP_

/**@brief Define set of common namespaces.
@details This macro is used for generating types and typelists.
((class name)("standard prefix")("IRI"))
*******************************************************************************/
#define OWLCPP_NAMESPACES_ALL \
   ((empty)("")("")) \
   ((blank)("_")("_")) \
   ((owl)("owl")("http://www.w3.org/2002/07/owl")) \
   ((rdfs)("rdfs")("http://www.w3.org/2000/01/rdf-schema")) \
   ((rdf)("rdf")("http://www.w3.org/1999/02/22-rdf-syntax-ns")) \
   ((xsd)("xsd")("http://www.w3.org/2001/XMLSchema")) \
/* */


//OWLCPP_STD_URI_TYPES

#endif /* URI_LIST_HPP_ */
