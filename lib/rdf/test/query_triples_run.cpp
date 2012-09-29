/** @file "/owlcpp/lib/rdf/test/query_triples_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE query_triples_run
#include "boost/test/unit_test.hpp"
#include <vector>
#include "test/exception_fixture.hpp"
#include "test/sample_triples.hpp"
#include "owlcpp/rdf/query_triples.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/** Iterate over members of RDF list
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts(Node_std::get(Nodes_owl()));
   sample_triples_01(ts);
   sample_triples_02(ts);
   Triple_store::doc_map_t::path_range r = ts.documents().find_path(path1);
   BOOST_REQUIRE(r);
   const Doc_id did1 = r.front();
   const Node_id nb1 = ts.insert_blank_node(did1, b1);
   const Node_id nb2 = ts.insert_blank_node(did1, b2);
   const Node_id ni1 = ts.insert_iri_node(iri11);
   const Node_id ni2 = ts.insert_iri_node(iri12);
   ts.insert_triple(nb1, terms::T_rdf_first::id(), ni1, did1);
   ts.insert_triple(nb1, terms::T_rdf_rest::id(), nb2, did1);
   ts.insert_triple(nb2, terms::T_rdf_first::id(), ni2, did1);
   ts.insert_triple(nb2, terms::T_rdf_rest::id(), terms::T_rdf_nil::id(), did1);

   Rdf_list_iter_s i(nb1, ts), i1;
   BOOST_CHECK(i != i1);
   BOOST_CHECK_EQUAL(*i++, ni1);
   BOOST_CHECK(i != i1);
   BOOST_CHECK_EQUAL(*i++, ni2);
   BOOST_CHECK(i == i1);

   const std::vector<Node_id> v1(
            boost::copy_range<std::vector<Node_id> >(rdf_list(nb1, ts))
   );

   ts.insert_triple(nb2, terms::T_rdf_rest::id(), terms::T_rdf_nil::id(), did1);
   BOOST_CHECK_THROW(
            boost::copy_range<std::vector<Node_id> >(rdf_list(nb1, ts)),
            Rdf_err
   );
}

}//namespace test
}//namespace owlcpp
