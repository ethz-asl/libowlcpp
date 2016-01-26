/** @file "/owlcpp/lib/rdf/test/object_id_01_cfail.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE object_id_01_cfail
#include "boost/test/unit_test.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( fail_obj_id ) {
   const Node_id id1(42);
   const Doc_id id2(42);
   if(id1 == id2) {
      //compilation error
   }
}

}//namespace test
}//namespace owlcpp
