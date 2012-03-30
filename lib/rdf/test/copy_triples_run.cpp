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

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts1( Node_map_std::get(Nodes_none()) );
   sample_triples_01(ts1);
   Triple_store ts2 = sample_triples_02();

   BOOST_CHECK( ts2.documents().find_path(path2) );
   BOOST_CHECK( ts1.documents().find_path(path1) );
   BOOST_CHECK( ! ts1.documents().find_path(path2) );
   BOOST_CHECK( ! ts2.documents().find_path(path1) );
   const std::size_t nt1 = ts1.triples().size();
   const std::size_t nt2 = ts2.triples().size();
   const int nb1 = boost::distance(ts1.nodes().find(terms::N_blank::id()));
   const int nb2 = boost::distance(ts2.nodes().find(terms::N_blank::id()));

   copy_triples(ts1, ts2);

   BOOST_CHECK_EQUAL(ts2.triples().size(), nt1 + nt2);
   BOOST_CHECK_EQUAL(
            boost::distance(ts2.nodes().find(terms::N_blank::id())),
            nb1 + nb2
   );

   BOOST_REQUIRE( ts2.documents().find_path(path1) );
   BOOST_REQUIRE( ts2.documents().find_path(path2) );
   const Doc_id did1 = ts2.documents().find_path(path1).front();
   const Doc_id did2 = ts2.documents().find_path(path2).front();
   BOOST_CHECK_EQUAL( ts2.path(did1), path1 );
   BOOST_CHECK_EQUAL( ts2.path(did2), path2 );
   BOOST_CHECK_EQUAL( ts2.ontology_iri(did1), doc1 );
   BOOST_CHECK_EQUAL( ts2.ontology_iri(did2), doc2 );
}

}//namespace test
}//namespace owlcpp
