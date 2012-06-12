/** @file "/owlcpp/lib/rdf/test/triple_map_detail_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_detail_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "boost/mpl/assert.hpp"

#include "owlcpp/rdf/triple_map.hpp"

namespace owlcpp{ namespace test{
using namespace owlcpp::detail;

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef Index_types<0,0,0,0> ind_types;
   typedef ind_types::store_tags i_tags;
   BOOST_MPL_ASSERT_RELATION(mpl::size<i_tags>::value, ==, 1);

   typedef Query_fb<1,0,0,0>::tags qt1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qt1>::value, ==, 1);
   typedef Search_types<i_tags, qt1>::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Main_store_tag>));
   typedef Search_types<i_tags, qt1>::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);

   typedef Query_fb<1,1,0,1>::tags qt2;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qt2>::value, ==, 3);
   typedef Search_types<i_tags, qt2>::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Main_store_tag>));
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef Index_types<0,0,1,1> ind_types;
   typedef ind_types::store_tags i_tags;
   BOOST_MPL_ASSERT_RELATION(mpl::size<i_tags>::value, ==, 3);

   typedef Query_fb<1,0,0,0>::tags qt1;
   typedef Search_types<i_tags, qt1>::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Main_store_tag>));
   typedef Search_types<i_tags, qt1>::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);
   typedef Search_types<i_tags, qt1>::query_second qs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs1>::value, ==, 1);

   typedef Query_fb<1,1,0,1>::tags qt2;
   typedef Search_types<i_tags, qt2>::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Doc_tag>));
   typedef Search_types<i_tags, qt2>::index_num sin2;
   BOOST_MPL_ASSERT_RELATION(sin2::value, ==, 1);
   typedef Search_types<i_tags, qt2>::query_second qs2;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs2>::value, ==, 2);

   typedef Query_fb<1,1,0,0>::tags qt3;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qt3>::value, ==, 2);
   typedef Search_types<i_tags, qt3>::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, Main_store_tag>));
   typedef Search_types<i_tags, qt3>::query_second qs3;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs3>::value, ==, 2);

   typedef Query_fb<0,0,1,1>::tags qt4;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qt4>::value, ==, 2);
   typedef Search_types<i_tags, qt4>::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, Obj_tag>));
   typedef Search_types<i_tags, qt4>::index_num sin4;
   BOOST_MPL_ASSERT_RELATION(sin4::value, ==, 2);
   typedef Search_types<i_tags, qt4>::query_second qs4;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs4>::value, ==, 1);
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef Index_types<1,1,1,1> ind_types;
   typedef ind_types::store_tags i_tags;
   BOOST_MPL_ASSERT_RELATION(mpl::size<i_tags>::value, ==, 5);

   typedef Query_fb<1,0,0,0>::tags qt1;
   typedef Search_types<i_tags, qt1>::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Subj_tag>));
   typedef Search_types<i_tags, qt1>::query_second qs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs1>::value, ==, 0);

   typedef Query_fb<1,1,0,1>::tags qt2;
   typedef Search_types<i_tags, qt2>::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Subj_tag>));
   typedef Search_types<i_tags, qt2>::query_second qs2;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs2>::value, ==, 2);

   typedef Query_fb<1,1,0,0>::tags qt3;
   typedef Search_types<i_tags, qt3>::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, Subj_tag>));

   typedef Query_fb<0,0,1,1>::tags qt4;
   typedef Search_types<i_tags, qt4>::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, Obj_tag>));

   typedef Query_fb<0,1,0,1>::tags qt5;
   typedef Search_types<i_tags, qt5>::index_tag si5;
   BOOST_MPL_ASSERT((boost::is_same<si5, Pred_tag>));

   typedef Query_fb<0,0,0,1>::tags qt6;
   typedef Search_types<i_tags, qt6>::index_tag si6;
   BOOST_MPL_ASSERT((boost::is_same<si6, Doc_tag>));

   typedef Query_fb<0,0,0,0>::tags qt7;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qt7>::value, ==, 0);
   typedef Search_types<i_tags, qt7>::index_tag si7;
   BOOST_MPL_ASSERT((boost::is_same<si7, Main_store_tag>));
   typedef Search_types<i_tags, qt7>::query_second qs7;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs7>::value, ==, 0);
}

}//namespace test
}//namespace owlcpp
