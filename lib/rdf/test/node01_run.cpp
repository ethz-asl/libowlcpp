/** @file "/owlcpp/lib/rdf/test/node01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node01_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/node.hpp"
#include "type_vector.hpp"
namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( node01_run_case01 ) {
   const Node n1 = Node(
            terms::T_owl_Nothing::ns_type::id(),
            terms::T_owl_Nothing::name()
   );
   BOOST_CHECK_EQUAL( n1, Node(terms::T_owl_Nothing()) );

   std::cout << n1 << std::endl;

}

}//namespace test
}//namespace owlcpp
