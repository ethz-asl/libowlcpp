/** @file "/owl_cpp/lib/test/factpp_01_run.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE factpp_01_run
#include "boost/test/unit_test.hpp"
#include <cassert>
#include <iostream>
#include "sample_data.hpp"
#include "factpp/Kernel_config.hpp"
#include "owl_cpp/triple_to_fact.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"
#include "node_type.hpp"
#include "owl_cpp/query_fact.hpp"
#include "owl_cpp/print.hpp"

namespace owl_cpp{ namespace test{

BOOST_AUTO_TEST_CASE( factpp_01_options ) {
   ReasoningKernel kernel;
//   kernel.getOptions()->printConfig(std::cout);
}


BOOST_AUTO_TEST_CASE( factpp_01_run_case01 ) {
   ReasoningKernel kernel;
   TExpressionManager& em = *kernel.getExpressionManager();
   const TDLConceptExpression* c1 = em.Concept("C1");
   const TDLConceptExpression* c2 = em.Concept("C2");
   kernel.impliesConcepts(c2, c1);

   assert( ! kernel.isKBPreprocessed() );
   assert( ! kernel.isKBClassified() );
   assert( ! kernel.isKBRealised() );

   assert( kernel.isKBConsistent() );//isKBConsistent() does pre-processing
   assert( kernel.isKBPreprocessed() );
   assert( ! kernel.isKBClassified() );
   assert( ! kernel.isKBRealised() );

   kernel.classifyKB();
   assert( kernel.isKBConsistent() );
   assert( kernel.isKBPreprocessed() );
   assert( kernel.isKBClassified() );
   assert( ! kernel.isKBRealised() );

   kernel.realiseKB();
   assert( kernel.isKBConsistent() );
   assert( kernel.isKBPreprocessed() );
   assert( kernel.isKBClassified() );
   assert( kernel.isKBRealised() );
}

//Tests disabled because of classifyKB() / realiseKB() issues
BOOST_AUTO_TEST_CASE( factpp_01_run_case03 ) {
   const std::string f_path = sample_file_path("test_01.owl");
   Triple_store store;
   load(f_path, store);
//   print(store, std::cout);
   ReasoningKernel kernel;
   add(store, kernel);

//   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( ! kernel.isKBPreprocessed() );
   BOOST_CHECK( ! kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() );

//   kernel.preprocessKB();
   BOOST_CHECK( kernel.isKBConsistent() );//isKBConsistent() does pre-processing
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( ! kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() );

   kernel.classifyKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() );

   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() );
}

BOOST_AUTO_TEST_CASE( factpp_01_run_case04 ) {
   const std::string f_path = sample_file_path("union_01.owl");
   Triple_store store;
   load(f_path, store);
//   print(store, std::cout);
   ReasoningKernel kernel;
   add(store, kernel);

//   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( ! kernel.isKBPreprocessed() );
   BOOST_CHECK( ! kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() );

//   kernel.preprocessKB();
   BOOST_CHECK( kernel.isKBConsistent() ); //isKBConsistent() does pre-processing
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( ! kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() );

   kernel.classifyKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() );

   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() );

   const TDLIndividualExpression* a_continuant =
         kernel.getExpressionManager()->Individual("a_continuant");
   const TDLConceptExpression* continuant_class =
         concept("http://www.ifomis.org/bfo/1.1/snap#Continuant", store, kernel);
   kernel.instanceOf(a_continuant, continuant_class);
   const TDLConceptExpression* entity_class =
         concept("http://www.ifomis.org/bfo/1.1#Entity", store, kernel);

   BOOST_CHECK( ! kernel.isKBPreprocessed() );
   BOOST_CHECK( ! kernel.isKBClassified() );
   BOOST_CHECK( ! kernel.isKBRealised() ); //there are instances now

   kernel.preprocessKB();
   kernel.classifyKB();
   kernel.realiseKB();
   BOOST_CHECK( kernel.isKBConsistent() );
   BOOST_CHECK( kernel.isKBPreprocessed() );
   BOOST_CHECK( kernel.isKBClassified() );
   BOOST_CHECK( kernel.isKBRealised() ); //there are instances now

   BOOST_CHECK( kernel.isInstance(a_continuant, entity_class) );
}


}//namespace test
}//namespace owl_cpp
