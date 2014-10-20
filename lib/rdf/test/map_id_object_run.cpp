/** @file "/owlcpp/lib/rdf/test/map_id_object_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_id_object_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/detail/map_id_object.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp{ namespace test{
namespace d = owlcpp::detail;

const Ns_id nsid1(42);
const Ns_id nsid2(43);
const Node_id nid0(13);
const Node_iri n1(nsid1, "n1");
const Node_iri n2(nsid1, "n2");
const Node_iri n3(nsid1, "n3");

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   d::Map_id_object<Node_iri, Node_id> mio1(nid0);
   BOOST_CHECK( ! mio1.find(nid0) );
   BOOST_CHECK_THROW(mio1.at(nid0), Rdf_err);
   const Node_id nid1 = mio1.insert(n1);
   BOOST_REQUIRE( mio1.find(nid1) );
   BOOST_CHECK_EQUAL(n1, *mio1.find(nid1));
   BOOST_CHECK_EQUAL(mio1[nid1], n1);
   BOOST_CHECK_EQUAL(mio1.at(nid1), n1);
   BOOST_CHECK_EQUAL(nid1, mio1.insert(n1));

   const Node_id nid2 = mio1.insert(n2);

   d::Map_id_object<Node_iri, Node_id> mio2(Node_id(0));
   mio2 = mio1;

   //insert existing node with new ID
   BOOST_CHECK_THROW(
            mio2.insert(Node_id(42), n1),
            Rdf_err
   );

   //insert new node with existing ID
   BOOST_CHECK_THROW(
            mio2.insert(nid2, n3),
            Rdf_err
   );

   //insert existing node with its current ID (NOP)
   BOOST_CHECK_NO_THROW( mio2.insert(nid2, n2); );

   //insert new node with ID below id0
   const Node_id nid01(1);
   BOOST_CHECK_THROW(
            mio2.insert(nid01, n3),
            Rdf_err
   );
}

}//namespace test
}//namespace owlcpp
