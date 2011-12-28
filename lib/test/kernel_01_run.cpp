/** @file "/owl_cpp/lib/test/kernel_01_run.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#define BOOST_TEST_MODULE kernel_01_run
#include "boost/test/unit_test.hpp"
#include "factpp/Kernel.hpp"
#include "boost/exception/diagnostic_information.hpp"
namespace b = boost;

namespace owl_cpp{ namespace test{

/** cardinality restriction on transitive roles:
cannot be done in tableau-based reasoners
*******************************************************************************/
BOOST_AUTO_TEST_CASE( kernel_01_run_case01 ) {
   ReasoningKernel kernel;
   TExpressionManager& em = *kernel.getExpressionManager();

   const TDLConceptExpression* car = em.Concept("Car");
   const TDLConceptExpression* motor = em.Concept("motor");
   const TDLObjectRoleExpression* has_part = em.ObjectRole("has_part");
   kernel.setTransitive( has_part ); //comment to avoid exception

   const TDLConceptExpression* restr = em.Cardinality(1, has_part,  motor);

   kernel.impliesConcepts(restr, car);

   BOOST_CHECK_THROW( kernel.isKBConsistent(), std::exception );
/*
   try{
      BOOST_CHECK( kernel.isKBConsistent() );
   } catch(...) {
      BOOST_FAIL(b::current_exception_diagnostic_information());
   }
*/
}

}//namespace test
}//namespace owl_cpp
