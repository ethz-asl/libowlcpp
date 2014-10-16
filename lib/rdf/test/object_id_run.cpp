/** @file "/owlcpp/lib/rdf/test/object_id_run.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE object_id
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "test/exception_fixture.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_obj_id ) {
   Node_id id1(42);
   Node_id id2(42);
   Node_id id3(0);
   BOOST_CHECK(id1 == id2);
   BOOST_CHECK(id1 != id3);
   BOOST_CHECK(id1 > id3);

   std::cout << "id1 " << id1 << " hash " << hash_value(id1) << std::endl;
}

}//namespace test
}//namespace owlcpp
