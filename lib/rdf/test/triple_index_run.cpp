/** @file "/owlcpp/lib/rdf/test/triple_index_run.cpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE triple_index_run
#include "boost/test/unit_test.hpp"
#include "boost/foreach.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/equal.hpp"

#include "test/exception_fixture.hpp"
#include "test/test_utils.hpp"
#include "owlcpp/rdf/detail/fragment_set.hpp"
#include "owlcpp/rdf/detail/triple_index_2.hpp"
#include "owlcpp/rdf/detail/convert_fragment.hpp"
#include "owlcpp/rdf/detail/fragment_map_vector.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/rdf/detail/triple_index_selector.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace m = map_triple_detail;

/** Test fragment set
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef m::Fragment_set<Doc_id,Node_id,Node_id> fs_t;
   fs_t fs;
   typedef fs_t::value_type val_t;
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(2)) );
   fs.insert( val_t(Doc_id(2), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(2), Node_id(2), Node_id(1)) );
   fs.insert( val_t(Doc_id(2), Node_id(1), Node_id(2)) );
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(1), Node_id(1), Node_id(1)) );
   fs.insert( val_t(Doc_id(1), Node_id(2), Node_id(1)) );
   BOOST_CHECK_EQUAL(fs.size(), 6U);
   typedef fs_t::const_range range_t;
   range_t r1 = fs.find(any(), any(), any());
   BOOST_CHECK_EQUAL(r1.size(), 6U);
   range_t r2 = fs.find(Doc_id(1), any(), any());
   BOOST_CHECK_EQUAL(r2.size(), 3U);
   range_t r3 = fs.find(Doc_id(1), Node_id(2), any());
   BOOST_CHECK_EQUAL(r3.size(), 1U);

   fs_t::result<Doc_id,any,Node_id>::range r4 = fs.find(Doc_id(2), any(), Node_id(1));
   BOOST_CHECK_EQUAL(distance(r4), 2);
}

/** Test fragment converter
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef m::Convert_fragment<m::Obj_tag,m::Doc_tag,m::Pred_tag, m::Subj_tag>
   conv_t;
   const Triple t1 = triple(1,2,3,4);
   const Node_id i1 = conv_t::get_index(t1);
   BOOST_CHECK(i1 == Node_id(3));
   const conv_t::fragment f1 = conv_t::get_fragment(t1);
   BOOST_CHECK( f1 == conv_t::fragment(Doc_id(4), Node_id(2), Node_id(1)) );
   BOOST_CHECK( t1 == conv_t::get_triple(i1, f1) );
}

/** Test fragment map vector
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef m::Fragment_set<Doc_id,Node_id,Node_id> fs_t;
   typedef m::Fragment_map_vector<Node_id, fs_t> fmv_t;
   fmv_t fmv;
   fmv_t::const_iterator i = fmv.begin();
}

typedef m::Triple_index<
         m::Fragment_map_vector,
         m::Subj_tag, m::Pred_tag, m::Obj_tag, m::Doc_tag
         > index1;

/** Test triple index Subj, Pred, Obj, Doc
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   index1 ind;
   BOOST_CHECK( ind.empty() );
   ind.insert(triple(0,0,0,0));
   ind.insert(triple(0,0,0,0));
   ind.insert(triple(0,1,0,0));
   ind.insert(triple(0,0,1,0));
   BOOST_CHECK( ! ind.empty() );
   BOOST_CHECK_EQUAL(ind.size(), 3U);
   BOOST_CHECK_EQUAL(
            ind.find(Node_id(0), any(), any(), any()).size(),
            3
   );
   BOOST_CHECK_EQUAL(
            ind.find(Node_id(1), any(), any(), any()).size(),
            0
   );
   BOOST_CHECK(
            boost::distance(ind.find(any(), any(), any(), any())) == 3
   );
   BOOST_CHECK_EQUAL(
            boost::distance(ind.find(any(), Node_id(1), any(), any())),
            1
   );

   BOOST_FOREACH(const Triple t, ind) {
      std::cout << t << std::endl;
   }
   std::cout << std::endl;

   index1::result<any,any,any,any>::range r = ind.find(any(), any(), any(), any());
   for(index1::const_iterator i = r.begin(), end = r.end(); i != end; ++i) {
      std::cout << *i << std::endl;
   }
   std::cout << std::endl;

   BOOST_FOREACH(const Triple t, ind.find(any(), any(), any(), any())) {
      std::cout << t << std::endl;
   }
}

typedef m::Triple_index<
         m::Fragment_map_vector,
         m::Obj_tag, m::Doc_tag, m::Subj_tag, m::Pred_tag
         > index2;

/** Test triple index Obj, Doc, Subj, Pred
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case05 ) {
   index2 ind;
   BOOST_CHECK( ind.empty() );
   ind.insert(triple(0,0,0,0));
   ind.insert(triple(0,0,0,0));
   ind.insert(triple(0,1,0,0));
   ind.insert(triple(0,0,1,0));
   BOOST_CHECK( ! ind.empty() );
   BOOST_CHECK_EQUAL(ind.size(), 3U);
   BOOST_CHECK_EQUAL(
            ind.find(any(), any(), Node_id(0), any()).size(),
            2
   );

   BOOST_FOREACH(const Triple t, ind) {
      std::cout << t << std::endl;
   }
//   BOOST_ERROR("");
}

/** Test optimality
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case06 ) {
   typedef m::Search_efficiency<
            boost::mpl::vector4<m::Obj_tag, m::Doc_tag, m::Subj_tag, m::Pred_tag>,
            boost::mpl::vector4<any, any, Node_id, any>
            > optim1;
   BOOST_MPL_ASSERT_RELATION( optim1::value, ==, 300 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<m::Subj_tag, m::Obj_tag, m::Doc_tag, m::Pred_tag>,
            boost::mpl::vector4<any, Node_id, Node_id, any>
            > optim2;
   BOOST_MPL_ASSERT_RELATION( optim2::value, ==, 30 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<m::Subj_tag, m::Obj_tag, m::Doc_tag, m::Pred_tag>,
            boost::mpl::vector4<any, any, Node_id, Doc_id>
            > optim3;
   BOOST_MPL_ASSERT_RELATION( optim3::value, ==, 31 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<m::Subj_tag, m::Doc_tag, m::Obj_tag, m::Pred_tag>,
            boost::mpl::vector4<any, any, Node_id, Doc_id>
            > optim4;
   BOOST_MPL_ASSERT_RELATION( optim4::value, ==, 13 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<m::Pred_tag, m::Doc_tag, m::Obj_tag, m::Subj_tag>,
            boost::mpl::vector4<any, any, Node_id, Doc_id>
            > optim5;
   BOOST_MPL_ASSERT_RELATION( optim5::value, ==, 13 );
}

typedef m::Triple_index<
         m::Fragment_map_vector,
         m::Pred_tag, m::Obj_tag, m::Doc_tag, m::Subj_tag
         > index3;

typedef m::Triple_index<
         m::Fragment_map_vector,
         m::Pred_tag, m::Doc_tag, m::Obj_tag, m::Subj_tag
         > index4;

/** Test index selector
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case07 ) {
   typedef boost::mpl::vector4<index1,index2,index3,index4> indices;

   typedef m::Index_selector<
      indices,
      boost::mpl::vector4<any, any, Node_id, any>
   > selector1;
   BOOST_MPL_ASSERT_RELATION( selector1::index::value, ==, 1 );

   typedef m::Index_selector<
      indices,
      boost::mpl::vector4<Node_id, any, Node_id, any>
   > selector2;
   BOOST_MPL_ASSERT_RELATION( selector2::index::value, ==, 0 );

   typedef m::Index_selector<
      indices,
      boost::mpl::vector4<any, Node_id, any, Doc_id>
   > selector3;
   BOOST_MPL_ASSERT_RELATION( selector3::index::value, ==, 3 );

}

}//namespace test
}//namespace owlcpp
