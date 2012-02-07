/** @file "/owlcpp/lib/rdf/test/triple_map_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_run
#include "boost/test/unit_test.hpp"
#include "boost/range.hpp"
#include "test/exception_fixture.hpp"

#include "owlcpp/rdf/triple_map.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

template<class Seq> void insert(Triple_map& tm, Seq const& s) {
   tm.insert(Triple(Node_id(s[0]), Node_id(s[1]), Node_id(s[2]), Doc_id(s[3])));
}

template<class Seq> Triple_map sample_tm(Seq const& s) {
   Triple_map tm;
   typedef typename boost::range_iterator<const Seq>::type iter_t;
   for(int i = 0; i != boost::size(s); ++i) insert(tm, s[i]);
   return tm;
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   const unsigned t[][4] = {
            {0,1,0,0},
            {0,2,0,0},
            {0,3,0,0}
   };
   const Triple_map tm = sample_tm(t);
   BOOST_CHECK( tm.find(blank(), blank(), blank(), blank()) );
   BOOST_CHECK( tm.find(Node_id(0), blank(), blank(), blank()) );
   BOOST_CHECK( tm.find(Node_id(0), blank(), Node_id(0), blank()) );
   BOOST_CHECK( tm.find(blank(), blank(), blank(), Doc_id(0)) );
   BOOST_CHECK( tm.find(Node_id(0), Node_id(2), Node_id(0), Doc_id(0)) );
   BOOST_CHECK( ! tm.find(Node_id(0), blank(), Node_id(1), blank()) );
}

}//namespace test
}//namespace owlcpp
