/** @file "/owlcpp/lib/rdf/test/node_store01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node_store01_run
#include "boost/test/unit_test.hpp"
#include "test/exception_translator.hpp"
#include "owlcpp/rdf/node_store.hpp"
#include "type_vector.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_translator );

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( node_store01_run_case01 ) {
   Node_store ns = Node_store::owl();

   BOOST_CHECK_GT(ns.iri_store().size(), 4u);
   BOOST_CHECK_GT(ns.size(), 50u);

   const Node_id nid1 = ns.insert(
            Node( terms::T_owl_Class::ns_type::id(), terms::T_owl_Class::name() )
   );
   BOOST_CHECK_EQUAL(nid1, terms::T_owl_Class::id());

   BOOST_CHECK_EQUAL(
            ns[terms::T_rdfs_range::id()],
            Node( terms::T_rdfs_range::ns_type::id(), terms::T_rdfs_range::name() )
   );
}

}//namespace test
}//namespace owlcpp
