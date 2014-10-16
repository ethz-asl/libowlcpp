/** @file "/owlcpp/lib/rdf/test/copy_triples_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE copy_triples_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_triples.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/copy_triples.hpp"
#include "test_utils.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_copy_triples_01 ) {
   Triple_store ts1(( Nodes_none() ));
//   Triple_store ts1;
   sample_triples_01(ts1);
   Triple_store ts2 = sample_triples_02();

   BOOST_CHECK( ts2.map_doc().find_path(path2) );
   BOOST_CHECK( ts1.map_doc().find_path(path1) );
   BOOST_CHECK( ! ts1.map_doc().find_path(path2) );
   BOOST_CHECK( ! ts2.map_doc().find_path(path1) );
   const std::size_t nt1 = ts1.map_triple().size();
   const std::size_t nt2 = ts2.map_triple().size();
//   const int nb1 = boost::distance(ts1.map_node().find(terms::blank::id()));
//   const int nb2 = boost::distance(ts2.map_node().find(terms::blank::id()));

   copy_triples(ts1, ts2);

   BOOST_CHECK_EQUAL(ts2.map_triple().size(), nt1 + nt2);
//   BOOST_CHECK_EQUAL(
//            boost::distance(ts2.map_node().find(terms::blank::id())),
//            nb1 + nb2
//   );

   BOOST_REQUIRE( ts2.map_doc().find_path(path1) );
   BOOST_REQUIRE( ts2.map_doc().find_path(path2) );
   const Doc_id did1 = ts2.map_doc().find_path(path1).front();
   const Doc_id did2 = ts2.map_doc().find_path(path2).front();
   BOOST_CHECK_EQUAL( ts2.at(did1).path, path1 );
   BOOST_CHECK_EQUAL( ts2.at(did2).path, path2 );
   BOOST_CHECK_EQUAL( ts2.at(did1).ontology_iri, *ts2.find_node_iri(doc1) );
   BOOST_CHECK_EQUAL( ts2.at(did2).ontology_iri, *ts2.find_node_iri(doc2) );
}

}//namespace test
}//namespace owlcpp
