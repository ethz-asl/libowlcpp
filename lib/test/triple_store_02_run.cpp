/** @file "/owl_cpp/lib/test/triple_store_02_run.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_02_run
#include "boost/test/unit_test.hpp"
#include <iostream>

#include "sample_data.hpp"

#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/query_triples.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;
#include "node_type.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "triple_store_checks.hpp"

namespace owl_cpp{ namespace test{

/** Find members of RDF collection
*******************************************************************************/
BOOST_AUTO_TEST_CASE( collection_01 ) {
   Triple_store store;
   load(sample_file_path("test_01.owl"), store);

   const std::vector<Node_id> c1 = rdf_collection(find_node("_", "0_2", store), store);
   BOOST_CHECK_EQUAL(c1.size(), 2U);

   //not a collection
   BOOST_CHECK_THROW(
         rdf_collection(find_node("_", "0_1", store), store),
         base_exception
   );
}

/** Check short names
*******************************************************************************/
BOOST_AUTO_TEST_CASE( short_name_case01 ) {
   Catalog cat;
   find_ontologies(cat, sample_file_path(""));
   Triple_store store;
   const std::string owl_iri("http://purl.obolibrary.org/obo/ido/dev/test_01.owl");
   load(sample_file_path("test_01.owl"), store, cat);

   BOOST_CHECK( check_and_print(store, std::cout) );

   print(store, std::cout);

   const Node_id C1_id = find_node(owl_iri, "C1", store);
   const std::string C1_sn = short_name(C1_id, store);
   BOOST_CHECK( C1_id == short_name_id(C1_sn, store) );
}

}//namespace test
}//namespace owl_cpp
