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
#include "boost/mpl/vector_c.hpp"

#include "test/exception_fixture.hpp"
#include "test/test_utils.hpp"
#include "owlcpp/rdf/detail/triple_set.hpp"
#include "owlcpp/rdf/detail/triple_index.hpp"
#include "owlcpp/rdf/detail/convert_fragment.hpp"
#include "owlcpp/rdf/detail/fragment_map_vector.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/rdf/detail/triple_index_selector.hpp"
#include "owlcpp/rdf/triple_tags.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

namespace m = map_triple_detail;

/** Test triple set 1
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   typedef m::Triple_set<Subj_tag, Obj_tag, Pred_tag> ts_t;
   ts_t ts;
   BOOST_CHECK(ts.empty());
   ts.insert( triple(1, 1, 2, 0) );
   insert_seq(ts, random_triples1);
   BOOST_CHECK_EQUAL(ts.size(), 21U);
   ts.erase(triple(1, 1, 2, 0));
   BOOST_CHECK_EQUAL(ts.size(), 20U);
   BOOST_CHECK_THROW(
            ts.erase(triple(1, 1, 2, 0)),
            Rdf_err
   );

   BOOST_CHECK_EQUAL(ts[0], triple(0,5,3,1));
   BOOST_CHECK_EQUAL(ts[10], triple(5,0,2,7));

   BOOST_CHECK_EQUAL(
            ts.find(
                     Node_id(5), //subj
                     any(), //obj
                     any() //pred
            ).size(),
            3
   );

   BOOST_CHECK_EQUAL(
            distance(
                     ts.find(
                              Node_id(6), //subj
                              any(), //obj
                              Node_id(3) //pred
                     )
            ),
            2
   );

   BOOST_CHECK_EQUAL(
            ts.find(
                     Node_id(2), //subj
                     Node_id(0), //obj
                     any()
            ).size(),
            2
   );

/*
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

   fs_t::query<Doc_id,any,Node_id>::range r4 = fs.find(Doc_id(2), any(), Node_id(1));
   BOOST_CHECK_EQUAL(distance(r4), 2);
*/
}

/** Test triple set 2
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02 ) {
   typedef m::Triple_set<Pred_tag, Subj_tag, Obj_tag> ts_t;
   ts_t ts;
   BOOST_CHECK(ts.empty());
   ts.insert( triple(1, 1, 2, 0) );
   insert_seq(ts, random_triples1);
   BOOST_CHECK_EQUAL(ts.size(), 21U);
   ts.erase(triple(1, 1, 2, 0));
   BOOST_CHECK_EQUAL(ts.size(), 20U);
   BOOST_CHECK_THROW(
            ts.erase(triple(1, 1, 2, 0)),
            Rdf_err
   );

   BOOST_CHECK_EQUAL(ts[0], triple(1,0,6,4));
   BOOST_CHECK_EQUAL(ts[10], triple(0,4,8,2));

   BOOST_CHECK_EQUAL(
            ts.find(
                     Node_id(5), //pred
                     any(), //subj
                     any() //obj
            ).size(),
            4
   );

   BOOST_CHECK_EQUAL(
            distance(
                     ts.find(
                              Node_id(1), //pred
                              any(), //subj
                              Node_id(0) //obj
                     )
            ),
            2
   );

   BOOST_CHECK_EQUAL(
            ts.find(
                     Node_id(3), //pred
                     Node_id(6), //subj
                     any() //obj
            ).size(),
            2
   );
}

/** Test fragment converter
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case02a ) {
   typedef m::Convert_fragment<Obj_tag,Doc_tag,Pred_tag, Subj_tag>
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
         Subj_tag, Pred_tag, Obj_tag, Doc_tag
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

   index1::query<any,any,any,any>::range r = ind.find(any(), any(), any(), any());
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
         Obj_tag, Doc_tag, Subj_tag, Pred_tag
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
            boost::mpl::vector4<Obj_tag, Doc_tag, Subj_tag, Pred_tag>,
            boost::mpl::vector4_c<bool, 0, 0, 1, 0>
            > optim1;
   BOOST_MPL_ASSERT_RELATION( optim1::value, ==, 100 + 3 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<Subj_tag, Obj_tag, Doc_tag, Pred_tag>,
            boost::mpl::vector4_c<bool, 0, 1, 1, 0>
            > optim2;
   BOOST_MPL_ASSERT_RELATION( optim2::value, ==, -4 + 10 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<Subj_tag, Obj_tag, Doc_tag, Pred_tag>,
            boost::mpl::vector4_c<bool, 0, 0, 1, 1>
            > optim3;
   BOOST_MPL_ASSERT_RELATION( optim3::value, ==, -4 + 10 + 1 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<Subj_tag, Doc_tag, Obj_tag, Pred_tag>,
            boost::mpl::vector4_c<bool, 0, 0, 1, 1>
            > optim4;
   BOOST_MPL_ASSERT_RELATION( optim4::value, ==, -4 + 10 + 1 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<Pred_tag, Doc_tag, Obj_tag, Subj_tag>,
            m::Boolean_signature<any, any, Node_id, Doc_id>::type
            > optim5;
   BOOST_MPL_ASSERT_RELATION( optim5::value, ==, 9 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<Subj_tag, Pred_tag, Obj_tag, Doc_tag>,
            boost::mpl::vector4_c<bool, 1, 0, 1, 0>
            > optim6;
   BOOST_MPL_ASSERT_RELATION( optim6::value, ==, 100 + 4 );

   typedef m::Search_efficiency<
            boost::mpl::vector4<Obj_tag, Subj_tag, Pred_tag, Doc_tag>,
            boost::mpl::vector4_c<bool, 1, 0, 1, 0>
            > optim7;
   BOOST_MPL_ASSERT_RELATION( optim7::value, ==, 100 + 3 + 10 );
}

typedef m::Triple_index<
         m::Fragment_map_vector,
         Pred_tag, Obj_tag, Doc_tag, Subj_tag
         > index3;

typedef m::Triple_index<
         m::Fragment_map_vector,
         Pred_tag, Doc_tag, Obj_tag, Subj_tag
         > index4;

/** Test index selector
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case07 ) {
   typedef boost::mpl::vector4<index1,index2,index3,index4> indices;

   typedef m::Index_selector<
      indices,
      boost::mpl::vector4_c<bool, 0, 0, 1, 0>
   > selector1;
   BOOST_MPL_ASSERT_RELATION( selector1::index::value, ==, 1 );

   typedef m::Index_selector<
      indices,
      boost::mpl::vector4_c<bool, 1, 0, 1, 0>
   > selector2;
   BOOST_MPL_ASSERT_RELATION( selector2::index::value, ==, 0 );

   typedef m::Index_selector<
      indices,
      m::Boolean_signature<any, Node_id, any, Doc_id>::type
   > selector3;
   BOOST_MPL_ASSERT_RELATION( selector3::index::value, ==, 3 );

}

}//namespace test
}//namespace owlcpp
