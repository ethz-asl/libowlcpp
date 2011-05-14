/** @file "/owl_cpp/lib/test/comparable_value_run.cpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE id_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "sample_data.hpp"
#include "owl_cpp/comparable_value.hpp"

namespace owl_cpp{ namespace test{

class Id1 : public Comparable_value<int>{
   Id1(const int x) : Comparable_value<int>(x) {}
   friend Id1 set_val(const int x);
};
Id1 set_val(const int x) {return Id1(x);}


BOOST_AUTO_TEST_CASE( id_01_run_case01 ) {
   Id1 id1 = set_val(42);
   Id1 id2 = set_val(42);
   Id1 id3 = set_val(-42);
   BOOST_CHECK(id1 == id2);
   BOOST_CHECK(id1 != id3);
   BOOST_CHECK(id1 > id3);
}

}//namespace test
}//namespace owl_cpp
