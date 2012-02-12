/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_store_query.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_QUERY_HPP_
#define TRIPLE_STORE_QUERY_HPP_
#include "boost/fusion/adapted/struct/adapt_struct.hpp"
#include "boost/fusion/container/vector/vector10.hpp"
#include "boost/fusion/include/mpl.hpp"
#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/at_c.hpp"
#include "boost/mpl/vector/vector10.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/erase.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/int.hpp"
#include "boost/mpl/contains.hpp"
#include "boost/mpl/empty.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/range.hpp"
#include "boost/type_traits/is_same.hpp"

#include "owlcpp/rdf/detail/triple_store_types.hpp"

BOOST_FUSION_ADAPT_STRUCT(
         owlcpp::Triple,
         (owlcpp::Node_id, subj_)
         (owlcpp::Node_id, pred_)
         (owlcpp::Node_id, obj_)
         (owlcpp::Doc_id, doc_)
)

namespace owlcpp{

struct blank{};

namespace query_detail{
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
//TODO: check that ranges form through equal_range method, where appropriate
/**
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc> class Query_type {

   //vector of active argument positions
   typedef mpl::vector0<> v0_t;
   typedef typename mpl::if_<
            typename boost::is_same<Subj,blank>::type,
            v0_t,
            typename mpl::push_back<v0_t, mpl::int_<0> >::type
            >::type v1_t;
   typedef typename mpl::if_<
            typename boost::is_same<Pred,blank>::type,
            v1_t,
            typename mpl::push_back<v1_t, mpl::int_<1> >::type
            >::type v2_t;
   typedef typename mpl::if_<
            typename boost::is_same<Obj,blank>::type,
            v2_t,
            typename mpl::push_back<v2_t, mpl::int_<2> >::type
            >::type v3_t;
   typedef typename mpl::if_<
            typename boost::is_same<Doc,blank>::type,
            v3_t,
            typename mpl::push_back<v3_t, mpl::int_<3> >::type
            >::type v4_t;

   // size 0 corresponds to template<> struct Query_type<blank,blank,blank,blank>;
   // explicit specialization should be used for that case
   BOOST_MPL_ASSERT_RELATION( mpl::size<v4_t>::type::value, >, 0 );

   typedef typename mpl::if_<
            typename mpl::contains<v4_t, mpl::int_<0> >::type,
            mpl::int_<0>,
            typename mpl::if_<
                  typename mpl::contains<v4_t, mpl::int_<2> >::type,
                  mpl::int_<2>,
                  typename mpl::if_<
                        typename mpl::contains<v4_t, mpl::int_<1> >::type,
                        mpl::int_<1>,
                        mpl::int_<3>
                  >::type
            >::type
   >::type nmi_t; //the number of multimap index to use
   typedef mpl::vector4<subj_tag, pred_tag, obj_tag, doc_tag> tags_vt;
   typedef typename mpl::at<tags_vt, nmi_t>::type tag_t;
   typedef typename triple_map_store_t::index<tag_t>::type index_t;
   typedef typename index_t::iterator iter1_t;

   typedef typename mpl::erase<
            v4_t,
            typename mpl::find<v4_t, nmi_t>::type
   >::type search_i_t; //search for these variables explicitly

   typedef fusion::vector4<Subj, Pred, Obj, Doc> search_args_t;

   struct Regular_range {
      typedef iter1_t iterator_t;
      typedef boost::iterator_range<iter1_t> range_t;
      static range_t make(iter1_t i1, iter1_t i2, search_args_t const&) {
         return boost::make_iterator_range(i1,i2);
      }
   };

   class Search_range {
      Search_range(search_args_t const& sa) : sa_(sa) {}

      template<class T> static bool compare(T const& t1, T const& t2) { return t1 == t2; }
      template<class T> static bool compare(T const&, blank const&) { return true; }

   public:
      typedef boost::filter_iterator<Search_range,iter1_t> iterator_t;
      typedef boost::iterator_range<iterator_t> range_t;

      static range_t make(iter1_t i1, iter1_t i2, search_args_t const& sa) {
         return boost::make_iterator_range(
                  iterator_t(Search_range(sa), i1, i2),
                  iterator_t(Search_range(sa), i2, i2)
         );
      }

      bool operator()(Triple const& triple) const {
         return
                  (
                           ! mpl::contains<search_i_t, mpl::int_<0> >::value ||
                           compare(fusion::at_c<0>(triple), fusion::at_c<0>(sa_))
                  ) &&
                  (
                           ! mpl::contains<search_i_t, mpl::int_<1> >::value ||
                           compare(fusion::at_c<1>(triple), fusion::at_c<1>(sa_))
                  ) &&
                  (
                           ! mpl::contains<search_i_t, mpl::int_<2> >::value ||
                           compare(fusion::at_c<2>(triple), fusion::at_c<2>(sa_))
                  ) &&
                  (
                           ! mpl::contains<search_i_t, mpl::int_<3> >::value ||
                           compare(fusion::at_c<3>(triple), fusion::at_c<3>(sa_))
                  )
                  ;
      }

   private:
      search_args_t const& sa_;

   };

   typedef typename mpl::if_<
            typename mpl::empty<search_i_t>::type,
            Regular_range,
            Search_range
   >::type range_maker_t;

public:
   typedef typename range_maker_t::iterator_t iterator_t;
   typedef typename range_maker_t::range_t range_t;

   static range_t range(
            triple_map_store_t const& tms, const Subj subj,
            const Pred pred, const Obj obj, const Doc doc
            ) {
      const search_args_t sa(subj, pred, obj, doc);
      iter1_t i1 = tms.get<tag_t>().find( fusion::at<nmi_t>(sa) );
      iter1_t i2 = tms.get<tag_t>().end();
      return range_maker_t::make( i1, i2, sa);
   }
};

/**
*******************************************************************************/
template<> struct Query_type<blank,blank,blank,blank> {
   typedef triple_map_store_t::iterator iterator_t;
   typedef boost::iterator_range<iterator_t> range_t;
   static range_t range(
            triple_map_store_t const& tms, const blank,
            const blank, const blank, const blank
            ) {
      return boost::make_iterator_range( tms.begin(), tms.end() );
   }
};

}//namespace query_detail
}//namespace owlcpp
#endif /* TRIPLE_STORE_QUERY_HPP_ */
