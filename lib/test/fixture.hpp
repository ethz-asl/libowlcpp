/** @file "/owl_cpp/lib/test/fixture.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#ifndef FIXTURE_HPP_
#define FIXTURE_HPP_
#include <string>
#include "boost/filesystem.hpp"
#include "boost/test/unit_test_monitor.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "owl_cpp/catalog.hpp"
#include "owl_cpp/triple_to_fact.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"

#ifndef SAMPLE_DATA_DIR
#error SAMPLE_DATA_DIR needs to be defined
#endif
#ifndef TEMPORARY_DIR
#error TEMPORARY_DIR needs to be defined
#endif

namespace owl_cpp{ namespace test{
namespace b = boost;
namespace but = boost::unit_test;
namespace bfs = boost::filesystem;

void translate(const b::exception &e) {
    BOOST_FAIL(b::diagnostic_information(e));
}

/** Test fixture for printing exception info
*******************************************************************************/
struct Fixture {
   Fixture() {
      but::unit_test_monitor.register_exception_translator<b::exception>(&translate);
   }

   static std::string sample_data_path(const std::string& file = "") {
      static const bfs::path path(BOOST_PP_STRINGIZE(SAMPLE_DATA_DIR));
      return (path / file).string();
   }

   static const std::string temp_path(const std::string& file = "") {
      static const std::string str(BOOST_PP_STRINGIZE(TEMPORARY_DIR));
      return (bfs::path(str) / file).string();
   }

};

}//namespace test
}//namespace owl_cpp
#endif /* FIXTURE_HPP_ */
