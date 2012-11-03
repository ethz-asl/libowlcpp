/** @file "/owlcpp/lib/rdf/test/id_tracker_run.cpp" 
part of %owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#define BOOST_TEST_MODULE id_tracker_run
#include "boost/test/unit_test.hpp"

#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/ns_id.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_id_tracker_01 ) {
   detail::Id_tracker<Ns_id> tracker( Ns_id(12) );
   BOOST_CHECK_EQUAL(tracker.get(), Ns_id(13));
   BOOST_CHECK_EQUAL(tracker.get(), Ns_id(14));
   tracker.push(Ns_id(14));
   tracker.push(Ns_id(13));
   //tracker.push(Ns_id(15)); //assert violation
   BOOST_CHECK_EQUAL(tracker.get(), Ns_id(13));
   BOOST_CHECK_EQUAL(tracker.get(), Ns_id(14));
}

}//namespace test
}//namespace owlcpp
