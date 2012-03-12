/** @file "/owlcpp/lib/rdf/test/node01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node01_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   const Node n1 = Node(
            terms::T_owl_Nothing::ns_type::id(),
            terms::T_owl_Nothing::name()
   );
   BOOST_CHECK_EQUAL( n1, Node(terms::T_owl_Nothing()) );

   std::cout << n1 << std::endl;

}

}//namespace test
}//namespace owlcpp
