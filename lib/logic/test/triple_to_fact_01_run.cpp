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

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_01 ) {
   Triple_store ts;
   load_file(sample_file_path("negative_property_assertion_01.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_02 ) {
   Triple_store ts;
   load_file(sample_file_path("og_01.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_03 ) {
   Triple_store ts;
   load_file(sample_file_path("og_02.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_04 ) {
   Sample_info const& si = sample_files()[5];
   BOOST_REQUIRE_NE(
            si.path.find("owl2-rl-rules-fp-differentFrom.owl"),
            std::string::npos
   );
   Triple_store ts;
   load_file(si.path, ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual(si.iri + "#Y1");
   const TDLIndividualExpression* i2 = em(k).Individual(si.iri + "#Y2");
   BOOST_CHECK( ! k.isSameIndividuals(i1, i2) );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_05 ) {
   Sample_info const& si = sample_files()[6];
   BOOST_REQUIRE_NE(
            si.path.find("owl2-rl-rules-fp-sameAs.owl"),
            std::string::npos
   );
   Triple_store ts;
   load_file(si.path, ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual(si.iri + "#X1");
   const TDLIndividualExpression* i2 = em(k).Individual(si.iri + "#X2");
   BOOST_CHECK( k.isSameIndividuals(i1, i2) );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_06 ) {
   Sample_info const& si = sample_files()[7];
   BOOST_REQUIRE_NE(
            si.path.find("owl2-rl-rules-ifp-differentFrom.owl"),
            std::string::npos
   );
   Triple_store ts;
   load_file(si.path, ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual(si.iri + "#X1");
   const TDLIndividualExpression* i2 = em(k).Individual(si.iri + "#X2");
   BOOST_CHECK( ! k.isSameIndividuals(i1, i2) );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_07 ) {
   Sample_info const& si = sample_files()[8];
   BOOST_REQUIRE_NE(
            si.path.find("owl2-rl-rules-ifp-sameAs.owl"),
            std::string::npos
   );
   Triple_store ts;
   load_file(si.path, ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* i1 = em(k).Individual(si.iri + "#Y1");
   const TDLIndividualExpression* i2 = em(k).Individual(si.iri + "#Y2");
   BOOST_CHECK( k.isSameIndividuals(i1, i2) );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_08 ) {
   Triple_store ts;
   load_file(sample_file_path("one_eq_two_01.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_09 ) {
   Triple_store ts;
   load_file(sample_file_path("negative_property_assertion_01.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_10 ) {
   Sample_info const& si = sample_files()[9];
   BOOST_REQUIRE_NE(
            si.path.find("propertyChain_01.owl"),
            std::string::npos
   );
   Triple_store ts;
   load_file(si.path, ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
   const TDLIndividualExpression* stewie =   em(k).Individual(si.iri + "#Stewie");
   const TDLIndividualExpression* lois =     em(k).Individual(si.iri + "#Lois");
   const TDLIndividualExpression* carol =    em(k).Individual(si.iri + "#Carol");
   const TDLObjectRoleExpression* hasAunt =  em(k).ObjectRole(si.iri + "#hasAunt");

   BOOST_CHECK(   k.isRelated(stewie, hasAunt, carol) );
   BOOST_CHECK( ! k.isRelated(lois,   hasAunt, carol) );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_11 ) {
   Triple_store ts;
   load_file(sample_file_path("data_value_clash.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_to_fact_12 ) {
   Triple_store ts;
   load_file(sample_file_path("version_test_b.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

}//namespace test
}//namespace owlcpp
