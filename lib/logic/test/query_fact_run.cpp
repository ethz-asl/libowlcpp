/** @file "/owlcpp/lib/logic/test/query_fact_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE query_fact_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include <iostream>
#include "test/sample_data.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "factpp/Kernel.hpp"
#include "owlcpp/logic/reasoner/query_fact.hpp"

namespace owlcpp{ namespace test{

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_store ts;
   load_file(sample_file_path("og_01.owl"), ts);
   ReasoningKernel k;
   submit(ts, k, false);
   TDLConceptExpression*c = k.getExpressionManager()->Top();
   BOOST_FOREACH(std::string const& s, get_sub(c, k, false)) std::cout << s << std::endl;
   BOOST_FOREACH(std::string const& s, get_same(c, k)) std::cout << s << std::endl;
}

}//namespace test
}//namespace owlcpp
