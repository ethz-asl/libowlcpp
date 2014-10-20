/** @file "/owlcpp/lib/logic/test/triple_to_fact_error_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE triple_to_fact_error_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "factpp/Kernel.hpp"
#include <string>
#include <vector>
#include "boost/assign/list_of.hpp"
#include "boost/foreach.hpp"

namespace owlcpp{ namespace test{

const std::vector<std::string> logic_error_files =
         boost::assign::list_of<std::string>
("err_missing_def01.owl")
("err_missing_def02.owl")
("err_missing_def03.owl")
("err_distinctMembers.owl")
;

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_missing_def_01 ) {
   ReasoningKernel k;
   BOOST_FOREACH(std::string const& fn, logic_error_files) {
      const std::string file_name = sample_file_path(fn);
      Triple_store ts;
      load_file(file_name, ts);
      BOOST_CHECK_THROW( submit(ts, k, false), Logic_err );
   }
}

}//namespace test
}//namespace owlcpp
