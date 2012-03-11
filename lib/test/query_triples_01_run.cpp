/** @file "/owlcpp/lib/test/query_triples_01_run.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#define BOOST_TEST_MODULE query_triples_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "sample_data.hpp"
#include "owlcpp/rdf/query_nodes.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "owlcpp/print.hpp"
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/custom_terms.hpp"
#include "triple_store_checks.hpp"
#include "custom_terms.hpp"
#include "owlcpp/terms/term_tags.hpp"
namespace ot = owlcpp::terms;

namespace owlcpp{ namespace test{

/**

*******************************************************************************/
BOOST_AUTO_TEST_CASE( query_triples_01_run_case01 ) {
   Catalog cat;
   find_ontologies(cat, sample_file_path(""));
   Triple_store store;
   insert_namespaces<custom_iri_type_vector>(store);
   insert_terms<custom_term_type_vector>(store);
   load(sample_file_path("test_01.owl"), store, cat);
   BOOST_CHECK( check_and_print(store, std::cout) );

   //owl:Class is never a subject
   BOOST_CHECK(
         ! find_triples(ot::T_owl_Class::id(), any(), any(), store)
   );

   //owl:Class is never a predicate
   BOOST_CHECK(
         ! find_triples(any(), ot::T_owl_Class::id(), any(), store)
   );

   //owl:Class is always object
   BOOST_CHECK(
         find_triples(any(), any(), ot::T_owl_Class::id(), store)
   );

   BOOST_CHECK(
         ! find_triples(ot::T_rdf_type::id(), any(), ot::T_owl_Class::id(), store)
   );

   BOOST_CHECK(
         find_triples(any(), ot::T_rdf_type::id(), ot::T_owl_Class::id(), store)
   );

   BOOST_CHECK(
         find_triples(
               find_node("http://purl.obolibrary.org/obo/ido/dev/test_01.owl", "C3", store),
               ot::T_rdf_type::id(),
               ot::T_owl_Class::id(),
               store
         )
   );
}

}//namespace test
}//namespace owlcpp
