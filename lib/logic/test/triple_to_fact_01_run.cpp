/** @file "/owlcpp/lib/logic/test/triple_to_fact_01_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_to_fact_01_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace test{

TExpressionManager& em(ReasoningKernel& k) {return *k.getExpressionManager();}

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;
   load_file(sample_file_path("negative_property_assertion_01.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Triple_store ts;
   load_file(sample_file_path("og_01.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Triple_store ts;
   load_file(sample_file_path("og_02.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Triple_store ts;
   load_file(sample_file_path("owl2-rl-rules-fp-differentFrom.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual("http://owl2.test/rules/Y1");
   const TDLIndividualExpression* i2 = em(k).Individual("http://owl2.test/rules/Y2");
   BOOST_CHECK( ! k.isSameIndividuals(i1, i2) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case05 ) {
   Triple_store ts;
   load_file(sample_file_path("owl2-rl-rules-fp-sameAs.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual("http://owl2.test/rules/X1");
   const TDLIndividualExpression* i2 = em(k).Individual("http://owl2.test/rules/X2");
   BOOST_CHECK( k.isSameIndividuals(i1, i2) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case06 ) {
   Triple_store ts;
   load_file(sample_file_path("owl2-rl-rules-ifp-differentFrom.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual("http://owl2.test/rules/X1");
   const TDLIndividualExpression* i2 = em(k).Individual("http://owl2.test/rules/X2");
   BOOST_CHECK( ! k.isSameIndividuals(i1, i2) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case07 ) {
   Triple_store ts;
   load_file(sample_file_path("owl2-rl-rules-ifp-sameAs.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual("http://owl2.test/rules/Y1");
   const TDLIndividualExpression* i2 = em(k).Individual("http://owl2.test/rules/Y2");
   BOOST_CHECK( k.isSameIndividuals(i1, i2) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case08 ) {
   Triple_store ts;
   load_file(sample_file_path("one_eq_two_01.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case09 ) {
   Triple_store ts;
   load_file(sample_file_path("negative_property_assertion_01.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case10 ) {
   Triple_store ts;
   load_file(sample_file_path("propertyChain_01.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const std::string iri = "http://owl.semanticweb.org/page/New-Feature-ObjectPropertyChain-001";
   const TDLIndividualExpression* stewie =   em(k).Individual(iri + "#Stewie");
   const TDLIndividualExpression* lois =     em(k).Individual(iri + "#Lois");
   const TDLIndividualExpression* carol =    em(k).Individual(iri + "#Carol");
   const TDLObjectRoleExpression* hasAunt =  em(k).ObjectRole(iri + "#hasAunt");

   BOOST_CHECK(   k.isRelated(stewie, hasAunt, carol) );
   BOOST_CHECK( ! k.isRelated(lois,   hasAunt, carol) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case11 ) {
   Triple_store ts;
   load_file(sample_file_path("data_value_clash.owl"), ts);
   ReasoningKernel k;
   submit_triples(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

}//namespace test
}//namespace owlcpp
