/** @file "/owlcpp/lib/rdf/test/map_node_iri_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_node_iri_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_node_iri.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp{ namespace test{

namespace t = owlcpp::terms;

const Ns_id nsid2(42);
const Ns_id nsid3(43);
const Node_id nid0(13);
const Node_id nid1(42);
const Node_iri n2(nsid2, "blah");

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_node_iri_01 ) {
   Map_node_iri mni1(nid0);
   BOOST_CHECK( ! mni1.find(nid1) );

   const Node_id nid2 = mni1.insert(n2);
   BOOST_REQUIRE( mni1.find(nid2) );
   BOOST_CHECK_EQUAL(mni1[nid2], n2);
   std::cout << n2 << std::endl;

   const Node_id nid2a = mni1.insert_iri(nsid2, "blah");
   BOOST_CHECK( mni1.find(nid2a) );

   BOOST_CHECK_EQUAL(nid2, nid2a);

   const Node_id nid3 = mni1.insert_iri(nsid3, "blah");
   BOOST_CHECK( mni1.find(nid3) );

   BOOST_CHECK_NE(nid2, nid3);

   Map_node_iri mni2 = mni1;
   BOOST_CHECK_EQUAL(mni1.size(), mni2.size());

   mni1.remove(nid2);
   BOOST_CHECK( ! mni1.find(n2) );
   BOOST_CHECK( ! mni1.find(nid2) );

   BOOST_CHECK(   mni2.find(n2) );
   BOOST_CHECK(   mni2.find(nid2) );
   BOOST_CHECK_EQUAL(mni2[nid2].ns_id(), nsid2);
   BOOST_CHECK_EQUAL(mni2[nid2], n2);

   const Node_id id4 = mni1.insert_iri(Ns_id(13), "blahblah");
   BOOST_CHECK( mni1.find(id4) );
   BOOST_CHECK_EQUAL(nid2, id4); //ID got recycled
}

}//namespace test
}//namespace owlcpp
