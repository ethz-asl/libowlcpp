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
   typedef Store_config<0,0,0,0> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 1);

   typedef Qargs_bool<1,0,0,0>::type qargs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qargs1>::value, ==, 4);
   BOOST_MPL_ASSERT_RELATION(Count_qargs<qargs1>::value, ==, 1);
   typedef Search_config<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Main_store_tag>));
   typedef sc1::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(Count_qargs<sc1::q2_tags>::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);

   typedef Qargs_bool<1,1,0,1>::type qargs2;
   typedef Search_config<sconfig, qargs2> sc2;
   BOOST_MPL_ASSERT_RELATION(Count_qargs<sc2::q2_tags>::value, ==, 3);
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Main_store_tag>));
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef Store_config<0,0,1,1> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 3);

   typedef Qargs_bool<1,0,0,0>::type qargs1;
   typedef Search_config<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Main_store_tag>));
   typedef sc1::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);
   typedef sc1::q2_tags qs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs1>::value, ==, 1);

   typedef Qargs_bool<1,1,0,1>::type qargs2;
   typedef Search_config<sconfig, qargs2> sc2;
   BOOST_MPL_ASSERT_RELATION(Count_qargs<sc2::q2_tags>::value, ==, 2);
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Doc_tag>));
   typedef sc2::index_num sin2;
   BOOST_MPL_ASSERT_RELATION(sin2::value, ==, 1);
   typedef sc2::q2_tags qs2;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs2>::value, ==, 2);

   typedef Qargs_bool<1,1,0,0>::type qargs3;
   typedef Search_config<sconfig, qargs3> sc3;
   typedef sc3::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, Main_store_tag>));
   typedef sc3::q2_tags qs3;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs3>::value, ==, 2);

   typedef Qargs_bool<0,0,1,1>::type qargs4;
   typedef Search_config<sconfig, qargs4> sc4;
   typedef sc4::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, Obj_tag>));
   typedef sc4::index_num sin4;
   BOOST_MPL_ASSERT_RELATION(sin4::value, ==, 2);
   typedef sc4::q2_tags qs4;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs4>::value, ==, 1);
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef Store_config<1,1,1,1> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 5);

   typedef Qargs_bool<1,0,0,0>::type qargs1;
   typedef Search_config<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Subj_tag>));
   typedef sc1::q2_tags qs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs1>::value, ==, 0);

   typedef Qargs_bool<1,1,0,1>::type qargs2;
   typedef Search_config<sconfig, qargs2> sc2;
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Subj_tag>));
   typedef sc2::q2_tags qs2;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs2>::value, ==, 2);

   typedef Qargs_bool<1,1,0,0>::type qargs3;
   typedef Search_config<sconfig, qargs3> sc3;
   typedef sc3::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, Subj_tag>));

   typedef Qargs_bool<0,0,1,1>::type qargs4;
   typedef Search_config<sconfig, qargs4> sc4;
   typedef sc4::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, Obj_tag>));

   typedef Qargs_bool<0,1,0,1>::type qargs5;
   typedef Search_config<sconfig, qargs5> sc5;
   typedef sc5::index_tag si5;
   BOOST_MPL_ASSERT((boost::is_same<si5, Pred_tag>));

   typedef Qargs_bool<0,0,0,1>::type qargs6;
   typedef Search_config<sconfig, qargs6> sc6;
   typedef sc6::index_tag si6;
   BOOST_MPL_ASSERT((boost::is_same<si6, Doc_tag>));

   typedef Qargs_bool<0,0,0,0>::type qargs7;
   typedef Search_config<sconfig, qargs7> sc7;
   typedef sc7::index_tag si7;
   BOOST_MPL_ASSERT((boost::is_same<si7, Main_store_tag>));
   typedef sc7::q2_tags qs7;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qs7>::value, ==, 0);
}

}//namespace test
}//namespace owlcpp
