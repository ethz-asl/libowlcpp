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
using namespace owlcpp::triple_map_detail;

BOOST_GLOBAL_FIXTURE( Exception_fixture );



/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef Store_config<0,0,0,0> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 1);

   typedef Deduce_args<1,0,0,0>::type qargs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qargs1>::value, ==, 4);
   BOOST_MPL_ASSERT_RELATION(Count_defined<qargs1>::value, ==, 1);
   typedef Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Main_store_tag>));
   typedef sc1::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc1::q2_args>::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);

   typedef Deduce_args<1,1,0,1>::type qargs2;
   typedef Search_config_v<sconfig, qargs2> sc2;
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc2::q2_args>::value, ==, 3);
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Main_store_tag>));
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef Store_config<0,0,1,1> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 3);

   typedef Deduce_args<1,0,0,0>::type qargs1;
   typedef Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Main_store_tag>));
   typedef sc1::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc1::q2_args>::value, ==, 1);

   typedef Deduce_args<1,1,0,1>::type qargs2;
   typedef Search_config_v<sconfig, qargs2> sc2;
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc2::q2_args>::value, ==, 2);
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Doc_tag>));
   typedef sc2::index_num sin2;
   BOOST_MPL_ASSERT_RELATION(sin2::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc2::q2_args>::value, ==, 2);

   typedef Deduce_args<1,1,0,0>::type qargs3;
   typedef Search_config_v<sconfig, qargs3> sc3;
   typedef sc3::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, Main_store_tag>));
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc3::q2_args>::value, ==, 2);

   typedef Deduce_args<0,0,1,1>::type qargs4;
   typedef Search_config_v<sconfig, qargs4> sc4;
   typedef sc4::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, Obj_tag>));
   typedef sc4::index_num sin4;
   BOOST_MPL_ASSERT_RELATION(sin4::value, ==, 2);
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc4::q2_args>::value, ==, 1);

   typedef Deduce_args<0,0,0,1>::type qargs5;
   typedef Search_config_v<sconfig, qargs5> sc5;
   typedef sc5::index_tag si5;
   BOOST_MPL_ASSERT((boost::is_same<si5, Doc_tag>));
   typedef sc5::index_num sin5;
   BOOST_MPL_ASSERT_RELATION(sin5::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc5::q2_args>::value, ==, 0);
   BOOST_MPL_ASSERT((boost::is_same<sc5::iterator, Element_index::const_iterator>));
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef Store_config<1,1,1,1> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 5);

   typedef Deduce_args<1,0,0,0>::type qargs1;
   typedef Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Subj_tag>));
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc1::q2_args>::value, ==, 0);

   typedef Deduce_args<1,1,0,1>::type qargs2;
   typedef Search_config_v<sconfig, qargs2> sc2;
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, Subj_tag>));
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc2::q2_args>::value, ==, 2);

   typedef Deduce_args<1,1,0,0>::type qargs3;
   typedef Search_config_v<sconfig, qargs3> sc3;
   typedef sc3::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, Subj_tag>));

   typedef Deduce_args<0,0,1,1>::type qargs4;
   typedef Search_config_v<sconfig, qargs4> sc4;
   typedef sc4::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, Obj_tag>));

   typedef Deduce_args<0,1,0,1>::type qargs5;
   typedef Search_config_v<sconfig, qargs5> sc5;
   typedef sc5::index_tag si5;
   BOOST_MPL_ASSERT((boost::is_same<si5, Pred_tag>));

   typedef Deduce_args<0,0,0,1>::type qargs6;
   typedef Search_config_v<sconfig, qargs6> sc6;
   typedef sc6::index_tag si6;
   BOOST_MPL_ASSERT((boost::is_same<si6, Doc_tag>));
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc6::q2_args>::value, ==, 0);
   BOOST_MPL_ASSERT((boost::is_same<sc6::iterator, Element_index::const_iterator>));

   typedef Deduce_args<0,0,0,0>::type qargs7;
   typedef Search_config_v<sconfig, qargs7> sc7;
   typedef sc7::index_tag si7;
   BOOST_MPL_ASSERT((boost::is_same<si7, Main_store_tag>));
   BOOST_MPL_ASSERT_RELATION(Count_defined<sc7::q2_args>::value, ==, 0);
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   typedef Store_config<0,1,0,0> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 2);

   typedef Deduce_args<0,1,0,0>::type qargs1;
   typedef Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, Pred_tag>));
   BOOST_MPL_ASSERT((boost::is_same<sc1::iterator, Element_index::const_iterator>));
   typedef Triple_map<0,1,0,0>::result_b<0,1,0,0>::type rt;
   BOOST_MPL_ASSERT((boost::is_same<rt, boost::iterator_range<Element_index::const_iterator> >));
}

}//namespace test
}//namespace owlcpp
