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
#include "owlcpp/rdf/detail/fragment_map_vector.hpp"
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

/** Test fragment map vector
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case03 ) {
   typedef m::Fragment_map_vector<Pred_tag, Subj_tag, Obj_tag, Doc_tag> fmv_t;
   fmv_t fmv;
   fmv_t::const_iterator i = fmv.begin();
}

typedef m::Triple_index<
         m::Fragment_map_vector,
         Subj_tag, Pred_tag, Obj_tag, Doc_tag
         > index1;

typedef m::Triple_index<
         m::Fragment_map_vector,
         Obj_tag, Doc_tag, Subj_tag, Pred_tag
         > index2;

typedef m::Triple_index<
         m::Fragment_map_vector,
         Pred_tag, Obj_tag, Doc_tag, Subj_tag
         > index3;

typedef m::Triple_index<
         m::Fragment_map_vector,
         Pred_tag, Doc_tag, Obj_tag, Subj_tag
         > index4;

typedef m::Triple_index<
         m::Fragment_map_vector,
         Subj_tag, Obj_tag, Doc_tag, Pred_tag
         > index5;

typedef m::Triple_index<
         m::Fragment_map_vector,
         Obj_tag, Subj_tag, Pred_tag, Doc_tag
         > index6;

/** Test triple index Subj, Pred, Obj, Doc
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case04 ) {
   index1 ind;
   BOOST_CHECK( ind.empty() );
   insert_seq(ind, random_triples1);
   BOOST_CHECK( ! ind.empty() );
   BOOST_CHECK_EQUAL(ind.size(), 20U);
   BOOST_CHECK_EQUAL(
            ind.find(Node_id(0), any(), any(), any()).size(),
            2
   );
   BOOST_CHECK_EQUAL(
            ind.find(Node_id(1), Node_id(1), any(), any()).size(),
            1
   );
   BOOST_CHECK(
            boost::distance(ind.find(any(), any(), any(), any())) == 20
   );
   BOOST_CHECK_EQUAL(
            boost::distance(ind.find(any(), Node_id(1), any(), any())),
            3
   );

   BOOST_FOREACH(Triple const& t, ind) {
      std::cout << t << std::endl;
   }
   std::cout << std::endl;

   index1::query<any,any,any,any>::range r = ind.find(any(), any(), any(), any());
   for(index1::const_iterator i = r.begin(), end = r.end(); i != end; ++i) {
      std::cout << *i << std::endl;
   }
   std::cout << std::endl;

   BOOST_FOREACH(Triple const& t, ind.find(any(), any(), any(), any())) {
      std::cout << t << std::endl;
   }
}

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

   BOOST_FOREACH(Triple const& t, ind) {
      std::cout << t << std::endl;
   }
//   BOOST_ERROR("");
}

/** Test optimality
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case06 ) {
   int e = index2::query<any,any,Node_id,any>::efficiency;
   BOOST_CHECK_EQUAL( e, 100 + 3 );

   e = index5::query<any,Node_id,Node_id,any>::efficiency;
   BOOST_CHECK_EQUAL( e, 10 - 4 );

   e = index5::query<any,any,Node_id,Doc_id>::efficiency;
   BOOST_CHECK_EQUAL( e, 15 - 4 );

   e = index4::query<any,any,Node_id,Doc_id>::efficiency;
   BOOST_CHECK_EQUAL( e, 15 - 2 );

   e = index1::query<Node_id,any,Node_id,any>::efficiency;
   BOOST_CHECK_EQUAL( e, 100 + 4 );

   e = index6::query<Node_id,any,Node_id,any>::efficiency;
   BOOST_CHECK_EQUAL( e, 100 + 3 + 10 );
}

/** Test index selector
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case07 ) {
   typedef boost::mpl::vector4<index1,index2,index3,index4> indices;

   typedef m::Index_selector<
      indices,
      any, any, Node_id, any
   > selector1;
   BOOST_MPL_ASSERT_RELATION( selector1::index::value, ==, 1 );

   typedef m::Index_selector<
      indices,
      Node_id, any, Node_id, any
   > selector2;
   BOOST_MPL_ASSERT_RELATION( selector2::index::value, ==, 0 );

   typedef m::Index_selector<
      indices,
      any, Node_id, any, Doc_id
   > selector3;
   BOOST_MPL_ASSERT_RELATION( selector3::index::value, ==, 3 );

}

}//namespace test
}//namespace owlcpp
