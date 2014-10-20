/** @file "/owlcpp/lib/logic/test/adaptor_triple_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE adaptor_triple_run
#include <iostream>
#include "boost/test/unit_test.hpp"
#include "boost/foreach.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_triple.hpp"
#include "owlcpp/io/input.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_triple_adaptor ) {
   Triple_store ts;
   load_file(sample_file_path("data_value_clash.owl"), ts);
   ReasoningKernel k;
   owlcpp::logic::factpp::Adaptor_triple at(ts, k, false);
   BOOST_FOREACH(Triple const& t, ts.map_triple()) {
      std::cout << to_string(t.subj_, ts) << std::endl;
      at.submit(t);
   }
}

}//namespace test
}//namespace owlcpp
