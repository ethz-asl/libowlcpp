/** @file "/owlcpp/lib/test/fixture.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#ifndef FIXTURE_HPP_
#define FIXTURE_HPP_
#include <string>
#include "boost/filesystem.hpp"
#include "boost/test/unit_test_monitor.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/reasoner/triple_to_fact.hpp"
#include "owlcpp/io/parse_to_triple_store.hpp"
#include "owlcpp/rdf/triple_store.hpp"

#ifndef SAMPLE_DATA_DIR
#error SAMPLE_DATA_DIR needs to be defined
#endif
#ifndef TEMPORARY_DIR
#error TEMPORARY_DIR needs to be defined
#endif

namespace owlcpp{ namespace test{ namespace detail{

void translate(boost::exception const& e) {
   BOOST_FAIL(boost::diagnostic_information(e));
}
}//namespace detail

/** Test fixture for printing exception info
*******************************************************************************/
struct Fixture {
   Fixture() {
      boost::unit_test::unit_test_monitor.register_exception_translator<boost::exception>(&detail::translate);
   }

   static std::string sample_data_path(const std::string& file = "") {
      static const boost::filesystem::path path(BOOST_PP_STRINGIZE(SAMPLE_DATA_DIR));
      return (path / file).string();
   }

   static const std::string temp_path(const std::string& file = "") {
      static const std::string str(BOOST_PP_STRINGIZE(TEMPORARY_DIR));
      return (boost::filesystem::path(str) / file).string();
   }

};

}//namespace test
}//namespace owlcpp
#endif /* FIXTURE_HPP_ */
