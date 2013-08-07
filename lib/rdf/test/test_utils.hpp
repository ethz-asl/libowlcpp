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

const unsigned random_triples1[][4] = {
      // random     Subj,Obj,Pred,Doc  Pred,Subj,Obj,Doc:
         {1,5,9,4}, //0   0  5  3  1     1  0  6  4
         {6,3,4,4}, //1   0  4  8  2     5  0  2  7
         {8,5,0,7}, //2   1  1  0  9     1  1  0  9
         {3,2,9,5}, //3   1  0  6  4     2  1  0  1
         {8,5,2,7}, //4   1  5  9  4     6  1  7  0
         {2,1,0,1}, //5   2  1  0  1     3  2  9  5
         {3,4,5,9}, //6   2  9  0  5     5  2  4  6
         {0,4,8,2}, //7   3  4  5  9     7  2  8  7
         {9,8,6,6}, //8   3  2  9  5     6  3  1  3
         {1,1,0,9}, //9   4  6  6  2     6  3  4  4
         {6,1,7,0}, //10  5  0  2  7     0  4  8  2
         {4,6,6,2}, //11  5  2  4  6     3  4  5  9
         {2,9,0,5}, //12  5  8  4  9     0  5  3  1
         {1,0,6,4}, //13  6  3  1  3     1  5  9  4
         {5,2,4,6}, //14  6  3  4  4     8  5  0  7
         {7,2,8,7}, //15  6  1  7  0     8  5  2  7
         {0,5,3,1}, //16  7  2  8  7     4  6  6  2
         {6,3,1,3}, //17  8  5  0  7     5  8  4  9
         {5,8,4,9}, //18  8  5  2  7     9  8  6  6
         {5,0,2,7}, //19  9  8  6  6     2  9  0  5
};

}//namespace test
}//namespace owlcpp
#endif /* TEST_UTILS_HPP_ */
