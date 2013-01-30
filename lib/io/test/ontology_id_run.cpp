/** @file "/owlcpp/lib/io/test/ontology_id_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE ontology_id_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/io/read_ontology_iri.hpp"
#include "boost/foreach.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/**@test Check that every ontology IRI and version is read correctly
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_ontology_id ) {
   BOOST_FOREACH(Sample_info const& si, sample_files()) {
      const std::pair<std::string,std::string> p1 = read_ontology_iri(si.path);
      BOOST_CHECK_EQUAL(p1.first, si.iri);
      BOOST_CHECK_EQUAL(p1.second, si.version);
   }
}

}//namespace test
}//namespace owlcpp
