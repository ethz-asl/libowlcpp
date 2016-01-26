/** @file "/owlcpp/lib/rdf/test/query_node_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE query_node_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{
namespace t = owlcpp::terms;

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_query_node ) {
   Triple_store ts;
   const Node_id nid1 =
            ts.insert_literal("1", to_string_full(t::xsd_boolean()));
   BOOST_CHECK( value<bool>(nid1, ts) );

   const Node_id nid2 =
            ts.insert_literal("true", to_string_full(t::xsd_boolean()));
   BOOST_CHECK( value<bool>(nid2, ts) );

   BOOST_CHECK_THROW(
            ts.insert_literal("TrUe", to_string_full(t::xsd_boolean())),
            Rdf_err
   );

   const Node_id nid4 =
            ts.insert_literal("10e300", to_string_full(t::xsd_double()));
   BOOST_CHECK_THROW( value<bool>(nid4, ts), Rdf_err );
   BOOST_CHECK_THROW( value<float>(nid4, ts), Rdf_err );
   BOOST_CHECK( value<double>(nid4, ts) == 10e300 );

   const Node_id nid5 =
            ts.insert_literal("255", to_string_full(t::xsd_double()));
   BOOST_CHECK_THROW( value<bool>(nid5, ts), Rdf_err );
   BOOST_CHECK_THROW( value<char>(nid5, ts), Rdf_err );
   BOOST_CHECK( value<double>(nid5, ts) == 255 );
   BOOST_CHECK( value<unsigned char>(nid5, ts) == 255 );

   const Node_id nid6 =
            ts.insert_literal("-10", to_string_full(t::xsd_double()));
   BOOST_CHECK_THROW( value<unsigned>(nid6, ts), Rdf_err );
   BOOST_CHECK( value<int>(nid6, ts) == -10 );
}

}//namespace test
}//namespace owlcpp
