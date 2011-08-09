/** @file "/owl_cpp/lib/test/triple_store_03_run.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_03_run
#include "boost/test/unit_test.hpp"
#include <iostream>

#include "sample_data.hpp"

#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;
#include "node_type.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/custom_terms.hpp"
#include "triple_store_checks.hpp"

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
typedef OWLCPP_NAMESPACE_MPL_VECTOR(OWLCPP_TEST_NAMESPACES) iri_type_vector;

/** Insert namespace prefixes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( ontology_prefixes_case01 ) {
   Triple_store store;
   const std::string iri1("http://www.blah.com/blah/blah");
   BOOST_CHECK_THROW(store.insert_namespace(iri1, "owl"), Triple_store::Err);
   BOOST_CHECK_NO_THROW(store.insert_namespace(iri1, "blah"));
   BOOST_CHECK_NO_THROW(store.insert_namespace(N_ro::name, N_ro::prefix));
   BOOST_CHECK(store[N_ro::name] == store.prefix_id(N_ro::prefix));
}

/** Adding custom namespaces to triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( triple_store_03_run_case02 ) {
   Triple_store store;
   BOOST_CHECK_THROW(store[N_ro::name], Triple_store::Err);
   insert_namespaces<iri_type_vector>(store);
   BOOST_CHECK( check_and_print(store, std::cout) );
   BOOST_REQUIRE_NO_THROW(store[N_ro::name]);
   BOOST_CHECK(store[N_ro::name] == store.prefix_id(N_ro::prefix));
   BOOST_CHECK(store["http://purl.obolibrary.org/obo/"] == store.prefix_id("obo"));
}

OWLCPP_GENERATE_TERM_TYPES(OWLCPP_TEST_TERMS)
OWLCPP_INIT_TERM_NAME_STRINGS(OWLCPP_TEST_TERMS)
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TEST_TERMS) term_type_vector;

/** Adding custom terms to triple store
*******************************************************************************/
BOOST_AUTO_TEST_CASE( triple_store_03_run_case03 ) {
   Triple_store store;
   BOOST_CHECK_THROW( store[N_ro::name], Triple_store::Err );
   BOOST_CHECK_THROW( find_node(T_obo_realizes(), store), Triple_store::Err );
   BOOST_CHECK( check_and_print(store, std::cout) );
   insert_namespaces<iri_type_vector>(store);
   insert_terms<term_type_vector>(store);
   BOOST_CHECK( check_and_print(store, std::cout) );
   BOOST_REQUIRE_NO_THROW( store[N_ro::name] );
   Node_id id1;
   BOOST_REQUIRE_NO_THROW( id1 = find_node(T_obo_realizes(), store) );
   BOOST_CHECK_EQUAL(store[id1].value_str(), T_obo_realizes::name);
}

}//namespace test
}//namespace owl_cpp
