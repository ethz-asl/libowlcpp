/** @file "/owl_cpp/lib/test/query_triples_01_run.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010-1 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE query_triples_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "sample_data.hpp"
#include "owl_cpp/query_nodes.hpp"
#include "owl_cpp/query_triples.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/custom_terms.hpp"
#include "triple_store_checks.hpp"
#include "custom_terms.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp{ namespace test{

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
         ! make_triples_query(ot::T_owl_Class::id(), blank(), blank(), store)
   );

   //owl:Class is never a predicate
   BOOST_CHECK(
         ! make_triples_query(blank(), ot::T_owl_Class::id(), blank(), store)
   );

   //owl:Class is always object
   BOOST_CHECK(
         make_triples_query(blank(), blank(), ot::T_owl_Class::id(), store)
   );

   BOOST_CHECK(
         ! make_triples_query(ot::T_rdf_type::id(), blank(), ot::T_owl_Class::id(), store)
   );

   BOOST_CHECK(
         make_triples_query(blank(), ot::T_rdf_type::id(), ot::T_owl_Class::id(), store)
   );

   BOOST_CHECK(
         make_triples_query(
               find_node("http://purl.obolibrary.org/obo/ido/dev/test_01.owl", "C3", store),
               ot::T_rdf_type::id(),
               ot::T_owl_Class::id(),
               store
         )
   );
}

}//namespace test
}//namespace owl_cpp
