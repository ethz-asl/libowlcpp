/** @file "/owlcpp/lib/rdf/test/map_std_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_std_run
#include "boost/test/unit_test.hpp"
#include "boost/concept_check.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/rdf/nodes_std.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

template<class Tag> Node_iri make_iri(Tag const&) {
   return Node_iri(Tag::ns_type::id(), Tag::fragment());
}

namespace t = owlcpp::terms;

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_std_01 ) {
   Map_std const& nms0 = Map_std::get(Nodes_none());
   boost::ignore_unused_variable_warning(nms0);
   Map_std const& nms1 = Map_std::get(Nodes_owl());
   boost::ignore_unused_variable_warning(nms1);
}

/**@test Standard node maps are singletons
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_std_02 ) {
   Map_std const& nms0 = Map_std::get(Nodes_none());
   Map_std const& nms1 = Map_std::get(Nodes_owl());
   Map_std const& nms2 = Map_std::get(Nodes_owl());
   BOOST_CHECK_NE(&nms0, &nms1); //different instances
   BOOST_CHECK_EQUAL(&nms1, &nms2); //same instance
}

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_map_std_03 ) {
   Map_std const& nms = Map_std::get(Nodes_owl());

   BOOST_CHECK_THROW(nms.at(Node_id(3000)), Rdf_err);
   BOOST_CHECK_THROW(nms.at(Node_id(3)), Rdf_err);
   BOOST_CHECK_NO_THROW(nms.at(t::empty_::id()));
   BOOST_CHECK_NO_THROW(nms.at(t::xsd_ENTITY::id()));
   BOOST_CHECK_NO_THROW(nms.at(t::xsd_positiveInteger::id()));

   BOOST_REQUIRE( nms.find(make_iri(t::empty_())) );
   BOOST_REQUIRE( nms.find(make_iri(t::owl_Class())) );
   BOOST_REQUIRE( nms.find(make_iri(t::owl_targetValue())) );
   BOOST_REQUIRE( nms.find(make_iri(t::rdfs_Datatype())) );
   BOOST_REQUIRE( nms.find(make_iri(t::rdf_type())) );
   BOOST_REQUIRE( nms.find(make_iri(t::xsd_dateTime())) );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::empty_())),
            t::empty_::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::owl_Class())),
            t::owl_Class::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::owl_targetValue())),
            t::owl_targetValue::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::rdfs_Datatype())),
            t::rdfs_Datatype::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::rdf_type())),
            t::rdf_type::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::xsd_dateTime())),
            t::xsd_dateTime::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::xsd_positiveInteger())),
            t::xsd_positiveInteger::id()
   );
}

}//namespace test
}//namespace owlcpp
