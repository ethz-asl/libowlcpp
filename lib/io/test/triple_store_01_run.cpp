/** @file "/owlcpp/lib/io/test/triple_store_01_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/raptor_wrapper.hpp"
#include "owlcpp/rdf/store_triple.hpp"
#include "raptor_to_store.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = sample_files()[13].path;
const std::string iri1 = sample_files()[13].iri;
const std::string ver1 = sample_files()[13].version;
const std::string path2 = sample_files()[0].path;
const std::string iri2 = sample_files()[0].iri;
const std::string ver2 = sample_files()[0].version;
const std::string path3 = sample_files()[3].path;
const std::string iri3 = sample_files()[3].iri;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case00 ) {
   Triple_store ts;
   BOOST_CHECK_EQUAL( ts.documents().size(), 0u );
   owlcpp::detail::Raptor_to_store rts(ts, path1, Check_both(iri1, ver1));
   boost::filesystem::ifstream ifs(path1);
   rts.parse(ifs);
   BOOST_REQUIRE_EQUAL( ts.documents().size(), 1u );
   const Doc_id did = *ts.documents().begin();
   BOOST_REQUIRE( ts.documents().find_path(path1));
   BOOST_CHECK_EQUAL( ts.documents().find_path(path1).front(), did);
   BOOST_CHECK_EQUAL( ts.path(did), path1 );
   const Node_id nid1 = ts.ontology_iri_id(did);
   const Node_id nid2 = ts.version_iri_id(did);
   Node const& node2 = ts.at(nid2);
   BOOST_CHECK( node2.value_str().empty() );
   BOOST_CHECK_EQUAL( ts.string(nid2), ver1 );
   Node const& node1 = ts.at(nid1);
   BOOST_CHECK( node1.value_str().empty() );
   BOOST_CHECK_EQUAL( ts.ontology_iri(did), iri1 );
   BOOST_CHECK_EQUAL( ts.triples().size(), 15u );
   BOOST_CHECK( ts.find_doc_iri(iri1) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;
   boost::filesystem::ifstream ifs(path1);
   load(ifs, ts);
   BOOST_CHECK_EQUAL( ts.triples().size(), 15u );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Triple_store ts;
   BOOST_CHECK( ! ts.find_doc_iri(iri1) );
   BOOST_CHECK( ! ts.find_doc_iri(iri2) );
   owlcpp::detail::Raptor_to_store rts(ts, path2, Check_both(iri2, ver2));
   boost::filesystem::ifstream ifs(path2);
   rts.parse(ifs);
   BOOST_CHECK( ! ts.find_doc_iri(iri1) );
   BOOST_CHECK( ts.find_doc_iri(iri2) );
   BOOST_CHECK_EQUAL(rts.imports().size(), 1u);
   BOOST_CHECK_EQUAL(rts.imports().at(0), ver1);
   BOOST_CHECK_EQUAL( ts.triples().size(), 3u );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   Triple_store ts;
   Catalog cat;
   cat.add(sample_file_path());
   load_file(path2, ts, cat);

   BOOST_CHECK_EQUAL( ts.triples().size(), 18u );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   Triple_store ts;
   Catalog cat;
   cat.add(sample_file_path());
   load_file(path3, ts, cat);

   BOOST_CHECK( ts.find_doc_iri(iri3) );
   BOOST_CHECK_GT( ts.triples().size(), 18u );
}

}//namespace test
}//namespace owlcpp
