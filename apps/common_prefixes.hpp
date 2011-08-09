/** @file "/owl_cpp/apps/common_prefixes.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
Copyright @author Mikhail K Levin @date 2010
*******************************************************************************/
#ifndef COMMON_PREFIXES_HPP_
#define COMMON_PREFIXES_HPP_

#include "owl_cpp/custom_terms.hpp"

#define OWLCPP_COMMON_NAMESPACES \
((obo)  ("http://purl.obolibrary.org/obo/")) \
((go)   ("http://purl.org/obo/owl/GO")) \
((ro)   ("http://www.obofoundry.org/ro/ro.owl")) \
((bfo)  ("http://www.ifomis.org/bfo/1.1")) \
((snap) ("http://www.ifomis.org/bfo/1.1/snap")) \
((span) ("http://www.ifomis.org/bfo/1.1/span")) \
((pro)  ("http://purl.org/obo/owl/PRO")) \
((chebi)("http://purl.org/obo/owl/CHEBI")) \
((rel)  ("http://purl.org/obo/owl/OBO_REL")) \
/* */
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_COMMON_NAMESPACES)


namespace owl_cpp{
OWLCPP_GENERATE_NAMESPACE_TYPES(OWLCPP_COMMON_NAMESPACES)

typedef OWLCPP_NAMESPACE_MPL_VECTOR(OWLCPP_COMMON_NAMESPACES) iri_type_vector;

}//namespace owl_cpp

#endif /* COMMON_PREFIXES_HPP_ */
