/** @file "/owlcpp/lib/rdf/test/map_std_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE map_std_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/rdf/nodes_std.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

template<class Tag> Node_iri make_iri(Tag const&) {
   return Node_iri(Tag::ns_type::id(), Tag::name());
}

namespace t = owlcpp::terms;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_std const& nms0 = Map_std::get(Nodes_none());
   Map_std const& nms1 = Map_std::get(Nodes_owl());

   BOOST_CHECK_EQUAL(nms0.ns_id_next()(), t::N_blank::index + 1 );
   BOOST_CHECK_EQUAL(nms0.node_id_next()(), t::T_empty_::index + 1);

   BOOST_CHECK_EQUAL(nms1.ns_id_next()(), t::N_owl::index + 1 );
   BOOST_CHECK_GT(nms1.node_id_next()(), 100);
}

/** Standard node classes are singletons
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Map_std const& nms0 = Map_std::get(Nodes_none());
   Map_std const& nms1 = Map_std::get(Nodes_owl());
   Map_std const& nms2 = Map_std::get(Nodes_owl());
   BOOST_CHECK_NE(&nms0, &nms1); //different instances
   BOOST_CHECK_EQUAL(&nms1, &nms2); //same instance
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Map_std const& nms = Map_std::get(Nodes_owl());

   BOOST_CHECK_THROW(nms.at(Node_id(3000)), Rdf_err);
   BOOST_CHECK_THROW(nms.at(Node_id(3)), Rdf_err);
   BOOST_CHECK_NO_THROW(nms.at(t::T_empty_::id()));
   BOOST_CHECK_NO_THROW(nms.at(t::T_xsd_ENTITY::id()));
   BOOST_CHECK_NO_THROW(nms.at(t::T_xsd_positiveInteger::id()));

   BOOST_REQUIRE( nms.find(make_iri(t::T_empty_())) );
   BOOST_REQUIRE( nms.find(make_iri(t::T_owl_Class())) );
   BOOST_REQUIRE( nms.find(make_iri(t::T_owl_targetValue())) );
   BOOST_REQUIRE( nms.find(make_iri(t::T_rdfs_Datatype())) );
   BOOST_REQUIRE( nms.find(make_iri(t::T_rdf_type())) );
   BOOST_REQUIRE( nms.find(make_iri(t::T_xsd_dateTime())) );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_empty_())),
            t::T_empty_::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_owl_Class())),
            t::T_owl_Class::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_owl_targetValue())),
            t::T_owl_targetValue::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_rdfs_Datatype())),
            t::T_rdfs_Datatype::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_rdf_type())),
            t::T_rdf_type::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_xsd_dateTime())),
            t::T_xsd_dateTime::id()
   );

   BOOST_CHECK_EQUAL(
            *nms.find(make_iri(t::T_xsd_positiveInteger())),
            t::T_xsd_positiveInteger::id()
   );
}

}//namespace test
}//namespace owlcpp
