/** @file "/owlcpp/lib/rdf/test/triple_map_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_run
#include "boost/test/unit_test.hpp"
#include "boost/range.hpp"
#include "test/exception_fixture.hpp"
#include <iostream>

#include "owlcpp/rdf/triple_map.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

template<class Seq> void insert(Triple_map& tm, Seq const& s) {
   tm.insert(Triple(Node_id(s[0]), Node_id(s[1]), Node_id(s[2]), Doc_id(s[3])));
}

template<class Seq> Triple_map sample_tm(Seq const& s) {
   Triple_map tm;
   typedef typename boost::range_iterator<const Seq>::type iter_t;
   for(std::size_t i = 0; i != boost::size(s); ++i) insert(tm, s[i]);
   return tm;
}

const unsigned t[][4] = {
         {0,1,0,0},
         {0,2,0,0},
         {0,3,0,0},
         {1,3,0,0},
         {0,3,1,0},
         {0,3,0,1},
         {0,4,0,0},
};
const Triple_map tm = sample_tm(t);

template<class ChT, class Tr> inline std::basic_ostream<ChT,Tr>& operator<<(
      std::basic_ostream<ChT,Tr>& os, Triple const& t) {
   return os << t.subj_ << ' ' << t.pred_ << ' ' << t.obj_ << ' ' << t.doc_;
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef Query<1,1,0,0>::range range_t;
   range_t r = tm.find(Node_id(0), Node_id(3), any(), any());
   range_t::iterator qi = r.begin();
   Triple_map::const_iterator i = tm.begin();

   std::cout << *i << std::endl;
   BOOST_CHECK( ! qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK( ! qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) ); //subject is not checked

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK(   qi.predicate()(*i++) );

   std::cout << *i << std::endl;
   BOOST_CHECK( ! qi.predicate()(*i++) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Query<0,0,0,0>::range r1 = tm.find(any(), any(), any(), any());
   BOOST_CHECK_EQUAL( distance(r1), 7 );

   Query<1,0,0,0>::range r2 = tm.find(Node_id(0), any(), any(), any());
   BOOST_CHECK_EQUAL( distance(r2), 6 );

   Query<1,0,1,0>::range r3 = tm.find(Node_id(0), any(), Node_id(0), any());
   BOOST_CHECK_EQUAL( distance(r3), 5 );

   Query<0,0,0,1>::range r4 = tm.find(any(), any(), any(), Doc_id(1));
   BOOST_CHECK_EQUAL( distance(r4), 1 );

   Query<1,1,1,1>::range r5 = tm.find(Node_id(0), Node_id(2), Node_id(0), Doc_id(0));
   BOOST_CHECK_EQUAL( distance(r5), 1 );

   Query<1,0,1,0>::range r6 = tm.find(Node_id(0), any(), Node_id(2), any());
   BOOST_CHECK_EQUAL( distance(r6), 0 );

   Query<0,0,1,0>::range r7 = tm.find(any(), any(), Node_id(0), any());
   BOOST_CHECK_EQUAL( distance(r7), 6 );

   Query<1,1,0,0>::range r8 = tm.find(Node_id(0), Node_id(3), any(), any());
   BOOST_CHECK_EQUAL( distance(r8), 3 );
}

}//namespace test
}//namespace owlcpp
