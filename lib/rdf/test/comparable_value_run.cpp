/** @file "/owlcpp/lib/rdf/test/comparable_value_run.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE id_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "test/sample_data.hpp"
#include "owlcpp/detail/comparable_value.hpp"

namespace owlcpp{ namespace test{

struct Id1 : public detail::Comparable_value<int>{
   Id1(const int x) : Comparable_value<int>(x) {}
};


BOOST_AUTO_TEST_CASE( id_01_run_case01 ) {
   Id1 id1(42);
   Id1 id2(42);
   Id1 id3(-42);
   BOOST_CHECK(id1 == id2);
   BOOST_CHECK(id1 != id3);
   BOOST_CHECK(id1 > id3);

   std::cout << "id1 " << id1 << " hash " << hash_value(id1) << std::endl;
}

}//namespace test
}//namespace owlcpp
