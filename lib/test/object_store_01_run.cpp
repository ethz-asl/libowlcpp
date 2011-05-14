/** @file "/owl_cpp/lib/test/object_store_01_run.cpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE object_store_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "boost/mpl/for_each.hpp"
namespace bmp = boost::mpl;
#include "boost/foreach.hpp"

#include "sample_data.hpp"
#include "owl_cpp/object_store.hpp"
#include "type_vector.hpp"
namespace ot = owl_cpp::terms;
#include "node_type.hpp"
#include "owl_cpp/node_id.hpp"

namespace owl_cpp {

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( object_store_01_run_case01 ) {
   Obj_store<Node_base> os_term;
   BOOST_CHECK(! os_term.find(Node_std<ot::T_rdf_Property>()) );
   BOOST_CHECK(! os_term.have( ot::T_rdf_Property::id() ) );
   os_term.insert( Node_std<ot::T_rdf_Property>(), ot::T_rdf_Property::id() );
   BOOST_CHECK( os_term.find(Node_std<ot::T_rdf_Property>()) );
   BOOST_CHECK( os_term.have( ot::T_rdf_Property::id() ) );

   Node_literal tl("blah");
   BOOST_CHECK( ! os_term.find(tl) );
   Node_id id_tl = os_term.insert(tl);
   BOOST_CHECK( os_term.find(tl) );
   BOOST_CHECK( os_term.have(id_tl) );
   BOOST_CHECK( os_term.insert(tl) == id_tl );
   BOOST_CHECK( os_term.get(id_tl) == tl );
}

/**
*******************************************************************************/
class Store_std_terms {
   mutable Obj_store<Node_base>& stor_;
public:
   Store_std_terms(Obj_store<Node_base>& stor) : stor_(stor) {}

   template<class T> void operator()(const T&) const {
      stor_.insert( Node_std<T>(), T::id() );
   }
};

/** Store standard nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( object_store_01_run_case02 ) {
   Obj_store<Node_base> nodes;
   Store_std_terms sso(nodes);
   bmp::for_each<ot::mpl_vector_terms_other_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_rdfs_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_rdf_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_owl1_t>(sso);
   bmp::for_each<ot::mpl_vector_terms_owl2_t>(sso);

   BOOST_CHECK_EQUAL(b::distance(nodes.objects()), b::distance(nodes.object_ids()));

   BOOST_FOREACH(const Node_id& id, nodes.object_ids()) {
      BOOST_CHECK(nodes.have(id));
   }

   BOOST_FOREACH(const Node_base& node, nodes.objects()) {
      BOOST_CHECK(nodes.find(node));
   }
}

}//namespace owl_cpp
