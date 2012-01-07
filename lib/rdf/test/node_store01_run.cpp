/** @file "/owlcpp/lib/rdf/test/node_store01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node_store01_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/node_store.hpp"
#include "owlcpp/terms/term_tags.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( node_store01_run_case01 ) {
   Node_store ns;

   BOOST_CHECK_EQUAL(ns.iri_store().size(), 0u);
   BOOST_CHECK_EQUAL(ns.size(), 0);

   const Node node1 =
            Node( terms::T_owl_Class::ns_type::id(), terms::T_owl_Class::name() );
   const Node_id nid1 = ns.insert(node1);
   const Node node2 = ns[nid1];
   BOOST_CHECK_EQUAL(
            node2,
            Node( terms::T_owl_Class::ns_type::id(), terms::T_owl_Class::name() )
   );

}

}//namespace test
}//namespace owlcpp
