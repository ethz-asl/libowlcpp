/** @file "/owlcpp/lib/rdf/test/map_triple_detail_run.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#define BOOST_TEST_MODULE triple_map_detail_run
#include "boost/test/unit_test.hpp"
#include "test/exception_fixture.hpp"
#include "boost/mpl/assert.hpp"

#include "owlcpp/rdf/map_triple.hpp"

namespace owlcpp{ namespace test{
namespace d = owlcpp::map_triple_detail;
namespace mpl = boost::mpl;

BOOST_GLOBAL_FIXTURE( Exception_fixture );



/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef d::Store_config<0,0,0,0> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 1);

   typedef d::Deduce_args<1,0,0,0>::type qargs1;
   BOOST_MPL_ASSERT_RELATION(mpl::size<qargs1>::value, ==, 4);
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<qargs1>::value, ==, 1);
   typedef d::Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, d::Main_store_tag>));
   typedef sc1::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc1::q2_args>::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);

   typedef d::Deduce_args<1,1,0,1>::type qargs2;
   typedef d::Search_config_v<sconfig, qargs2> sc2;
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc2::q2_args>::value, ==, 3);
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, d::Main_store_tag>));
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef d::Store_config<0,0,1,1> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 3);

   typedef d::Deduce_args<1,0,0,0>::type qargs1;
   typedef d::Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, d::Main_store_tag>));
   typedef sc1::index_num sin1;
   BOOST_MPL_ASSERT_RELATION(sin1::value, ==, 0);
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc1::q2_args>::value, ==, 1);

   typedef d::Deduce_args<1,1,0,1>::type qargs2;
   typedef d::Search_config_v<sconfig, qargs2> sc2;
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc2::q2_args>::value, ==, 2);
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, d::Doc_tag>));
   typedef sc2::index_num sin2;
   BOOST_MPL_ASSERT_RELATION(sin2::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc2::q2_args>::value, ==, 2);

   typedef d::Deduce_args<1,1,0,0>::type qargs3;
   typedef d::Search_config_v<sconfig, qargs3> sc3;
   typedef sc3::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, d::Main_store_tag>));
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc3::q2_args>::value, ==, 2);

   typedef d::Deduce_args<0,0,1,1>::type qargs4;
   typedef d::Search_config_v<sconfig, qargs4> sc4;
   typedef sc4::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, d::Obj_tag>));
   typedef sc4::index_num sin4;
   BOOST_MPL_ASSERT_RELATION(sin4::value, ==, 2);
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc4::q2_args>::value, ==, 1);

   typedef d::Deduce_args<0,0,0,1>::type qargs5;
   typedef d::Search_config_v<sconfig, qargs5> sc5;
   typedef sc5::index_tag si5;
   BOOST_MPL_ASSERT((boost::is_same<si5, d::Doc_tag>));
   typedef sc5::index_num sin5;
   BOOST_MPL_ASSERT_RELATION(sin5::value, ==, 1);
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc5::q2_args>::value, ==, 0);
   BOOST_MPL_ASSERT((boost::is_same<sc5::iterator, d::Element_index::const_iterator>));
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef d::Store_config<1,1,1,1> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 5);

   typedef d::Deduce_args<1,0,0,0>::type qargs1;
   typedef d::Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, d::Subj_tag>));
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc1::q2_args>::value, ==, 0);

   typedef d::Deduce_args<1,1,0,1>::type qargs2;
   typedef d::Search_config_v<sconfig, qargs2> sc2;
   typedef sc2::index_tag si2;
   BOOST_MPL_ASSERT((boost::is_same<si2, d::Subj_tag>));
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc2::q2_args>::value, ==, 2);

   typedef d::Deduce_args<1,1,0,0>::type qargs3;
   typedef d::Search_config_v<sconfig, qargs3> sc3;
   typedef sc3::index_tag si3;
   BOOST_MPL_ASSERT((boost::is_same<si3, d::Subj_tag>));

   typedef d::Deduce_args<0,0,1,1>::type qargs4;
   typedef d::Search_config_v<sconfig, qargs4> sc4;
   typedef sc4::index_tag si4;
   BOOST_MPL_ASSERT((boost::is_same<si4, d::Obj_tag>));

   typedef d::Deduce_args<0,1,0,1>::type qargs5;
   typedef d::Search_config_v<sconfig, qargs5> sc5;
   typedef sc5::index_tag si5;
   BOOST_MPL_ASSERT((boost::is_same<si5, d::Pred_tag>));

   typedef d::Deduce_args<0,0,0,1>::type qargs6;
   typedef d::Search_config_v<sconfig, qargs6> sc6;
   typedef sc6::index_tag si6;
   BOOST_MPL_ASSERT((boost::is_same<si6, d::Doc_tag>));
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc6::q2_args>::value, ==, 0);
   BOOST_MPL_ASSERT((boost::is_same<sc6::iterator, d::Element_index::const_iterator>));

   typedef d::Deduce_args<0,0,0,0>::type qargs7;
   typedef d::Search_config_v<sconfig, qargs7> sc7;
   typedef sc7::index_tag si7;
   BOOST_MPL_ASSERT((boost::is_same<si7, d::Main_store_tag>));
   BOOST_MPL_ASSERT_RELATION(d::Count_defined<sc7::q2_args>::value, ==, 0);
}

/** Select optimal triple index
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   typedef d::Store_config<0,1,0,0> sconfig;
   BOOST_MPL_ASSERT_RELATION(mpl::size<sconfig::tags>::value, ==, 2);

   typedef d::Deduce_args<0,1,0,0>::type qargs1;
   typedef d::Search_config_v<sconfig, qargs1> sc1;
   typedef sc1::index_tag si1;
   BOOST_MPL_ASSERT((boost::is_same<si1, d::Pred_tag>));
   BOOST_MPL_ASSERT((boost::is_same<sc1::iterator, d::Element_index::const_iterator>));
   typedef Map_triple<0,1,0,0>::result_b<0,1,0,0>::type rt;
   BOOST_MPL_ASSERT((boost::is_same<rt, boost::iterator_range<d::Element_index::const_iterator> >));
}

}//namespace test
}//namespace owlcpp
