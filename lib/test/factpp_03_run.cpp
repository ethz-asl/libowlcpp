/** @file "/owlcpp/lib/test/factpp_03_run.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE factpp_03_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "boost/exception/diagnostic_information.hpp"
#include "sample_data.hpp"
#include "triple_store_checks.hpp"
#include "factpp/Kernel.hpp"
#include "owlcpp/reasoner/triple_to_fact.hpp"
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "node_type.hpp"
#include "owlcpp/reasoner/query_fact.hpp"
#include "owlcpp/print.hpp"

namespace owlcpp { namespace test{

BOOST_AUTO_TEST_CASE( owl2_rl_rules_fp_differentFrom_owl_case ) {
   Triple_store store;
   load(sample_file_path("owl2-rl-rules-fp-differentFrom.owl"), store);
//   print(store, std::cout);
   BOOST_CHECK( check_and_print(store, std::cout) );
   ReasoningKernel kernel;
   BOOST_REQUIRE_NO_THROW( add(store, kernel) );

   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() );

   const TDLIndividualExpression* i1 =
         instance("http://owl2.test/rules/", "Y1", store, kernel);
   const TDLIndividualExpression* i2 =
         instance("http://owl2.test/rules/", "Y2", store, kernel);
   BOOST_CHECK( ! kernel.isSameIndividuals(i1, i2) );
}

BOOST_AUTO_TEST_CASE( owl2_rl_rules_fp_sameAs_owl_case ) {
   Triple_store store;
   load(sample_file_path("owl2-rl-rules-fp-sameAs.owl"), store);
//   print(store, std::cout);
   BOOST_CHECK( check_and_print(store, std::cout) );
   ReasoningKernel kernel;
   BOOST_REQUIRE_NO_THROW( add(store, kernel) );

   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() );

   const TDLIndividualExpression* i1 =
         instance("http://owl2.test/rules/", "X1", store, kernel);
   const TDLIndividualExpression* i2 =
         instance("http://owl2.test/rules/", "X2", store, kernel);
   BOOST_CHECK( kernel.isSameIndividuals(i1, i2) );
}

BOOST_AUTO_TEST_CASE( owl2_rl_rules_ifp_differentFrom_owl_case ) {
   Triple_store store;
   load(sample_file_path("owl2-rl-rules-ifp-differentFrom.owl"), store);
//   print(store, std::cout);
   BOOST_CHECK( check_and_print(store, std::cout) );
   ReasoningKernel kernel;
   BOOST_REQUIRE_NO_THROW( add(store, kernel) );

   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() );

   const TDLIndividualExpression* i1 =
         instance("http://owl2.test/rules/", "X1", store, kernel);
   const TDLIndividualExpression* i2 =
         instance("http://owl2.test/rules/", "X2", store, kernel);
   BOOST_CHECK( ! kernel.isSameIndividuals(i1, i2) );
}

BOOST_AUTO_TEST_CASE( owl2_rl_rules_ifp_sameAs_owl_case ) {
   Triple_store store;
   load(sample_file_path("owl2-rl-rules-ifp-sameAs.owl"), store);
//   print(store, std::cout);
   BOOST_CHECK( check_and_print(store, std::cout) );
   ReasoningKernel kernel;
   BOOST_REQUIRE_NO_THROW( add(store, kernel) );

   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() );

   const TDLIndividualExpression* i1 =
         instance("http://owl2.test/rules/", "Y1", store, kernel);
   const TDLIndividualExpression* i2 =
         instance("http://owl2.test/rules/", "Y2", store, kernel);
   BOOST_CHECK( kernel.isSameIndividuals(i1, i2) );
}

/** @details this test is slow in debug mode for some reason
*******************************************************************************/
BOOST_AUTO_TEST_CASE( one_eq_two_01_owl_case ) {
   Triple_store store;
   load(sample_file_path("one_eq_two_01.owl"), store);
//   print(store, std::cout);
   BOOST_CHECK( check_and_print(store, std::cout) );
   ReasoningKernel kernel;
   BOOST_REQUIRE_NO_THROW( add(store, kernel) );

   BOOST_CHECK( ! kernel.isKBConsistent() );
}

}//namespace test
}//namespace owlcpp
