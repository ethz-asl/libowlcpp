/** @file "/owl_cpp/lib/test/factpp_04_run.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE factpp_04_run
#include "boost/test/unit_test.hpp"

#include <iostream>
#include "boost/exception/diagnostic_information.hpp"
#include "sample_data.hpp"
#include "triple_store_checks.hpp"
#include "Kernel.h"
#include "owl_cpp/triple_to_fact.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"
#include "node_type.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/query_fact.hpp"

namespace owl_cpp{ namespace test{

BOOST_AUTO_TEST_CASE( negative_property_assertion_01_owl_case ) {
   Triple_store store;
   load(sample_file_path("negative_property_assertion_01.owl"), store);
//   print(store, std::cout);
   BOOST_CHECK( check_and_print(store, std::cout) );
   ReasoningKernel kernel;
   BOOST_REQUIRE_NO_THROW( add(store, kernel) );

   BOOST_CHECK( ! kernel.isKBConsistent() );
}

BOOST_AUTO_TEST_CASE( negative_property_assertion_case02 ) {
   ReasoningKernel kernel;
   TExpressionManager& em = *kernel.getExpressionManager();
   em.Individual("Peter");
   em.Individual("Meg");
   em.ObjectRole("hasSon");
   kernel.relatedTo(
         em.Individual("Peter"),
         em.ObjectRole("hasSon"),
         em.Individual("Meg")
   );
   kernel.relatedToNot(
         em.Individual("Peter"),
         em.ObjectRole("hasSon"),
         em.Individual("Meg")
   );
   BOOST_CHECK( ! kernel.isKBConsistent() );
}

BOOST_AUTO_TEST_CASE( propertyChain_01_owl_case ) {
   const std::string iri("http://owl.semanticweb.org/page/New-Feature-ObjectPropertyChain-001");
   Catalog cat;
   Triple_store store;
   ReasoningKernel kernel;
   try{
      find_ontologies(cat, sample_file_path(""));
      load(sample_file_path("propertyChain_01.owl"), store, cat);
      check(store);
      add(store, kernel);
   } catch(b::exception& e) {
      BOOST_FAIL(b::diagnostic_information(e));
   }

   BOOST_CHECK( kernel.isKBConsistent() );
   const Node_id stewie_id = find_node(iri, "Stewie", store);
   const TDLIndividualExpression* stewie_e = instance(stewie_id, store, kernel);
   const Node_id lois_id = find_node(iri, "Lois", store);
   const TDLIndividualExpression* lois_e = instance(lois_id, store, kernel);
   const Node_id carol_id = find_node(iri, "Carol", store);
   const TDLIndividualExpression* carol_e = instance(carol_id, store, kernel);
   const Node_id hasAunt_id = find_node(iri, "hasAunt", store);
   const TDLObjectRoleExpression* hasAunt_e = property(hasAunt_id, store, kernel);
   BOOST_CHECK( kernel.isRelated(stewie_e, hasAunt_e, carol_e) );
   BOOST_CHECK( ! kernel.isRelated(lois_e, hasAunt_e, carol_e) );
}

}//namespace test
}//namespace owl_cpp
