/** @file "/owl_cpp/lib/test/custom_terms.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef TEST_CUSTOM_TERMS_HPP_
#define TEST_CUSTOM_TERMS_HPP_
#include "owl_cpp/custom_terms.hpp"

#define OWLCPP_TEST_NAMESPACES \
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
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TEST_NAMESPACES)

#define OWLCPP_TEST_TERMS \
      ((snap)(MaterialEntity)) \
      ((span)(ProcessualEntity)) \
      ((ro)(has_participant)) \
      ((obo)(realizes)(OBI_0000308)) \
      ((rel)(inheres_in)) \
      ((rel)(has_output)(has-output)) \
/* */
#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TEST_TERMS)

namespace owl_cpp{ namespace test{
OWLCPP_GENERATE_NAMESPACE_TYPES(OWLCPP_TEST_NAMESPACES)
OWLCPP_INIT_NAMESPACE_NAME_STRINGS(OWLCPP_TEST_NAMESPACES)
typedef OWLCPP_NAMESPACE_MPL_VECTOR(OWLCPP_TEST_NAMESPACES) custom_iri_type_vector;

OWLCPP_GENERATE_TERM_TYPES(OWLCPP_TEST_TERMS)
OWLCPP_INIT_TERM_NAME_STRINGS(OWLCPP_TEST_TERMS)
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TEST_TERMS) custom_term_type_vector;

}//namespace test
}//namespace owl_cpp

#endif /* TEST_CUSTOM_TERMS_HPP_ */
