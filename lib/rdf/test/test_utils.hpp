/** @file "/owlcpp/lib/rdf/test/test_utils.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TEST_UTILS_HPP_
#define TEST_UTILS_HPP_
#include "boost/range.hpp"

#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace test{


Triple triple(const unsigned t0, const unsigned t1, const unsigned t2, const unsigned t3) {
   return Triple::make(Node_id(t0), Node_id(t1), Node_id(t2), Doc_id(t3));
}

Triple triple(unsigned const s[4]) {return triple(s[0], s[1], s[2], s[3]);}

template<class Tm, class Seq> void insert_triple(Tm& tm, Seq const& s) {
   tm.insert(triple(s));
}

template<class Tm, class Seq> void insert_seq(Tm& tm, Seq const& s) {
   for(std::size_t i = 0; i != boost::size(s); ++i) insert_triple(tm, s[i]);
}

}//namespace test
}//namespace owlcpp
#endif /* TEST_UTILS_HPP_ */
