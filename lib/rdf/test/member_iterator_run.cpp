/** @file "/owlcpp/lib/rdf/test/member_iterator_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE member_iterator_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include <map>
#include <vector>
#include "boost/range.hpp"
#include "boost/foreach.hpp"
#include "owlcpp/detail/member_iterator.hpp"
#include "owlcpp/detail/iterator_member_pair.hpp"
#include "boost/multi_index/detail/hash_index_iterator.hpp"

namespace owlcpp{ namespace test{

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_pair_iter ) {
   typedef std::map<int, double> map_t;

   typedef Iterator_member_pair<map_t::const_iterator, const double, 2> iter_t;
   typedef boost::iterator_range<iter_t> range_t;

   map_t map;
   map[1] = 2.5;
   map[42] = 42;
   range_t r1(map.begin(), map.end());
   BOOST_CHECK_EQUAL(distance(r1), 2);

   range_t r2(map.end(), map.end());
   BOOST_CHECK_EQUAL(distance(r2), 0);
}

struct A {int a; double b;};

/**@test
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_struct_iter ) {
   typedef std::vector<A> vector_t;
   typedef Member_iterator<
            vector_t::const_iterator,
            const double,
            &A::b
            > iter_t;
   typedef boost::iterator_range<iter_t> range_t;
   vector_t v;
   v.push_back(A());
   v.push_back(A());

   range_t r1(v.begin(), v.end());
   BOOST_CHECK_EQUAL(distance(r1), 2);

   range_t r2(v.end(), v.end());
   BOOST_CHECK_EQUAL(distance(r2), 0);
}

}//namespace test
}//namespace owlcpp
