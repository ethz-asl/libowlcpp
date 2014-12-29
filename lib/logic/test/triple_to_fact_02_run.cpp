/** @file "/owlcpp/lib/logic/test/triple_to_fact_02_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_to_fact_02_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_triple.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace test{

TExpressionManager& em(ReasoningKernel& k) {return *k.getExpressionManager();}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_data_property_assertion ) {
   Triple_store ts;
   load_file(sample_file_path("data_property_assertion_01.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_datatype_restriction ) {
   Triple_store ts;
   load_file(sample_file_path("datatype-min-max.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( ! k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_dp_restriction ) {
   Triple_store ts;
   load_file(sample_file_path("data_property_restriction.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   BOOST_CHECK( k.isKBConsistent() );
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_subclass_axiom ) {
   Triple_store ts;
   load_file(sample_file_path("subclass_axiom.owl"), ts);

   ReasoningKernel k;
   submit(ts, k, true);
   BOOST_CHECK( k.isKBConsistent() );

   const Doc_id doc = *ts.map_doc().begin();
   const Node_id nid = ts[doc].ontology_iri;
   const std::string ns_iri = to_string_full(nid, ts);

   const TDLIndividualExpression* b = em(k).Individual(ns_iri + "#b");
   const TDLIndividualExpression* c = em(k).Individual(ns_iri + "#c");
   const TDLIndividualExpression* bc = em(k).Individual(ns_iri + "#bc");

   /*
   FaCT v1.6.2 returns TDLConceptExpression*,
   FaCT v1.6.3 returns TDLConceptName*
    */
   const TDLConceptExpression* A = em(k).Concept(ns_iri + "#A");
   const TDLConceptExpression* B = em(k).Concept(ns_iri + "#B");
   const TDLConceptExpression* C = em(k).Concept(ns_iri + "#C");

   BOOST_CHECK( k.isInstance(b, B) );
   BOOST_CHECK( k.isInstance(c, C) );
   BOOST_CHECK( ! k.isInstance(b, A) );
   BOOST_CHECK( ! k.isInstance(c, A) );
   BOOST_CHECK( k.isInstance(bc, B) );
   BOOST_CHECK( k.isInstance(bc, C) );
   BOOST_CHECK( k.isInstance(bc, A) );
}

}//namespace test
}//namespace owlcpp
