/** @file "/owlcpp/lib/rdf/test/triple_map_01_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_01_run
#include "boost/test/unit_test.hpp"
#include "boost/range.hpp"
#include "test/exception_fixture.hpp"
#include <iostream>

#include "owlcpp/rdf/triple_map.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

template<class Tm, class Seq> void insert(Tm& tm, Seq const& s) {
   tm.insert( Node_id(s[0]), Node_id(s[1]), Node_id(s[2]), Doc_id(s[3]) );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Triple_map<> tm;
   const unsigned t[] = {0,1,0,0};
   insert(tm, t);
   /*Query<1,1,0,0>::range r =*/ tm.find(Node_id(0), Node_id(3), any(), any());
   /*Query<1,1,0,0>::range r =*/ tm.find(any(), Node_id(3), any(), any());
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   Triple_map<0,1,0,0> tm;
   const unsigned t[] = {0,1,0,0};
   insert(tm, t);
//   Query<1,1,0,0>::range r = tm.find(Node_id(0), Node_id(3), any(), any());
}

}//namespace test
}//namespace owlcpp
