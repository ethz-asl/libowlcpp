/** @file "/owl_cpp/lib/test/owl_std_types_01_run.cpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE owl_std_uri_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "sample_data.hpp"

//#include "owl_cpp/owl_std_uri_type.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp{ namespace test{

/** Standard namespace and term types store correct strings and indices
*******************************************************************************/
BOOST_AUTO_TEST_CASE( owl_std_types_01_run_case01 ) {
   BOOST_CHECK_EQUAL(ot::N_rdf::iri(), "http://www.w3.org/1999/02/22-rdf-syntax-ns");
   BOOST_CHECK_EQUAL(ot::T_owl_FunctionalProperty::name(), "FunctionalProperty");
   BOOST_CHECK(ot::N_owl::index != ot::N_empty::index);
   BOOST_CHECK(ot::N_owl::id() != ot::N_empty::id() );
   BOOST_CHECK(ot::N_empty::id() != ot::N_blank::id() );
}

/** Correct ID objects can be generated for each standard type (namespaces and terms)
*******************************************************************************/
BOOST_AUTO_TEST_CASE( make_ids ) {
   BOOST_CHECK( ot::N_owl::index == ot::N_owl::id()() );
   BOOST_CHECK( ot::N_owl::id_type(ot::N_owl::index) == ot::N_owl::id() );
   BOOST_CHECK( Ns_id(ot::N_owl::index) == ot::N_owl::id() );

   BOOST_CHECK( ot::T_rdf_type::index == ot::T_rdf_type::id()() );
   BOOST_CHECK( ot::T_rdf_type::id_type(ot::T_rdf_type::index) == ot::T_rdf_type::id() );
   BOOST_CHECK( Node_id(ot::T_rdf_type::index) == ot::T_rdf_type::id() );

   BOOST_CHECK( ot::N_owl::index == ot::N_owl::id()() );
   BOOST_CHECK( ot::T_empty_generic::index == ot::T_empty_generic::id()() );
   BOOST_CHECK( ot::T_rdf_type::index != ot::T_empty_generic::id()() );
   BOOST_CHECK( ot::T_rdf_type::index == ot::T_rdf_type::id()() );

   BOOST_CHECK( ot::N_rdfs::index == ot::T_rdfs_subClassOf::ns_type::id()() );
   BOOST_CHECK( ot::N_rdfs::id_type(ot::N_rdfs::index) == ot::T_rdfs_subClassOf::ns_type::id() );
   BOOST_CHECK( Ns_id(ot::N_rdfs::index) == ot::T_rdfs_subClassOf::ns_type::id() );
}

}//namespace test
}//namespace owl_cpp
