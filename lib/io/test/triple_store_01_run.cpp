/** @file "/owlcpp/lib/io/test/triple_store_01_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_store_01_run
#include "boost/test/unit_test.hpp"
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/parser_triple.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "adaptor_triple_store.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

const std::string path1 = sample_file_path("version_test_b.owl");
const std::string path2 = sample_file_path("imports_test_01.owl");
const std::string iri3 = "http://purl.obolibrary.org/obo/ido/dev/version_test_b.owl";

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
   owlcpp::detail::Adaptor_triple_store ats(ts, path2);
   Parser_triple parser;
   parser(path2, ats);
   BOOST_CHECK_EQUAL(ats.imports().size(), 1u);
   BOOST_CHECK_EQUAL(ats.imports().at(0), iri3);
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

}//namespace test
}//namespace owlcpp
