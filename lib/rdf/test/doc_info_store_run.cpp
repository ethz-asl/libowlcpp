/** @file "/owlcpp/lib/rdf/test/doc_info_store_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE doc_info_store_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/doc_info_store.hpp"
#include "owlcpp/rdf/node_store_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

const std::string path1 = "path1";
const std::string path2 = "path2";

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Doc_store ds;
   ds.insert(path1, Node_id(13), Node_id(0));
   BOOST_CHECK_THROW(
            ds.insert(path1, Node_id(12), Node_id(0)),
            Doc_store::Err
   );

   ds.insert(path2, Node_id(13), Node_id(1));
   Doc_id const* did = ds.find(Node_id(1));
   BOOST_REQUIRE(did);
   BOOST_CHECK_EQUAL(Node_id(13), ds.iri(*did));
   BOOST_CHECK_EQUAL(Node_id(1), ds.version(*did));
}

}//namespace test
}//namespace owlcpp
