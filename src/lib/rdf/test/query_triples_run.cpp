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

namespace t = owlcpp::terms;

/**@test Iterate over members of RDF list
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_query_triple ) {
   Triple_store ts;
   sample_triples_01(ts);
   sample_triples_02(ts);
   Triple_store::map_doc_type::path_range r = ts.map_doc().find_path(path1);
   BOOST_REQUIRE(r);
   const Doc_id did1 = r.front();
   const Node_id nb1 = ts.insert_blank(1, did1);
   const Node_id nb2 = ts.insert_blank(2, did1);
   const Node_id ni1 = ts.insert_node_iri(iri11);
   const Node_id ni2 = ts.insert_node_iri(iri12);
   ts.insert(Triple::make(nb1, t::rdf_first::id(), ni1, did1));
   ts.insert(Triple::make(nb1, t::rdf_rest::id(), nb2, did1));
   ts.insert(Triple::make(nb2, t::rdf_first::id(), ni2, did1));
   ts.insert(Triple::make(nb2, t::rdf_rest::id(), t::rdf_nil::id(), did1));

   Rdf_list_iter_s i(nb1, ts), i1;
   BOOST_CHECK(i != i1);
   BOOST_CHECK_EQUAL(*i++, ni1);
   BOOST_CHECK(i != i1);
   BOOST_CHECK_EQUAL(*i++, ni2);
   BOOST_CHECK(i == i1);

   const std::vector<Node_id> v1(
            boost::copy_range<std::vector<Node_id> >(rdf_list(nb1, ts))
   );

   ts.insert(Triple::make(nb2, t::rdf_rest::id(), nb1, did1));
   BOOST_CHECK_THROW(
            boost::copy_range<std::vector<Node_id> >(rdf_list(nb1, ts)),
            Rdf_err
   );
}

}//namespace test
}//namespace owlcpp
