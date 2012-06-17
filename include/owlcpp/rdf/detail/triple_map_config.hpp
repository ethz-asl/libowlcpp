/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_map_config.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_CONFIG_HPP_
#define TRIPLE_MAP_CONFIG_HPP_
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/include/all.hpp"
#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/make_vector.hpp"
#include "boost/fusion/include/mpl.hpp"
#include "boost/fusion/include/size.hpp"
#include "boost/fusion/include/push_back.hpp"
#include "boost/fusion/include/value_at.hpp"
#include "boost/fusion/include/zip.hpp"

#include "boost/iterator/filter_iterator.hpp"

#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/begin.hpp"
#include "boost/mpl/contains.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/erase.hpp"
#include "boost/mpl/eval_if.hpp"
#include "boost/mpl/find.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/size_t.hpp"
#include "boost/mpl/vector.hpp"
#include "boost/mpl/vector_c.hpp"

#include "boost/range.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/type_traits/has_operator.hpp"

#include "owlcpp/rdf/detail/triple_map_tags.hpp"

namespace owlcpp{ namespace detail{
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
namespace fusion_rof = boost::fusion::result_of;

template<class> struct Index;
typedef mpl::vector<Subj_tag, Pred_tag, Obj_tag, Doc_tag> triple_tags;

/** Make a vector of tags for each index to be created, including the tag for
the main store of triples.
The tags are listed in the order of increasing (projected) search performance.
For example, subject index is expected to be best performing and the possibility
for using it should be checked first; the main store is expected to have the worst
performance.
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> struct Store_config {
   typedef mpl::vector_c<bool,Subj,Pred,Obj,Doc> args_vector;
   //tags in the order of priority
   typedef mpl::vector<Doc_tag, Pred_tag, Obj_tag, Subj_tag> priority_tags;

   /** sequence of store tags in the order of increasing priority,
   starting from Main_store_tag */
   typedef typename mpl::fold<
            priority_tags,
            mpl::vector<Main_store_tag>,
            mpl::if_<
               mpl::at<args_vector, mpl::_2>,
               mpl::push_back<mpl::_1, mpl::_2>,
               mpl::_1
            >
   >::type tags;

   typedef typename mpl::fold<
            tags,
            fusion::vector<>,
            mpl::push_back<mpl::_1, Index<mpl::_2> >
   >::type store;
};

/**
*******************************************************************************/
template<class QArgs> struct Query_tags {
   typedef typename mpl::fold<
            triple_tags,
            mpl::vector<>,
            mpl::if_<
               boost::is_same<mpl::at<QArgs, mpl::_2>, mpl::at<Triple, mpl::_2> >,
               mpl::push_back<mpl::_1, mpl::_2>,
               mpl::_1
            >
   >::type type;
};

/** Extract range of triples from index
*******************************************************************************/
template<class Indx, class Tag = typename Indx::tag> struct Search_range1 {
   typedef typename Indx::const_range range;
   typedef Tag tag;

   template<class Subj, class Pred, class Obj, class Doc> static range
   get(Indx const& index,const Subj t1,const Pred t2,const Obj t3,const Doc t4) {
      return index.find(
               boost::fusion::at<tag>(
                        boost::fusion::make_vector(t1, t2, t3, t4)
               )
      );
   }
};

/** Specialize to extract range of triples from main store
*******************************************************************************/
template<class Indx> struct Search_range1<Indx, Main_store_tag> {
   typedef typename Indx::const_range range;

   template<class Subj, class Pred, class Obj, class Doc> static range
   get(Indx const& index, const Subj,const Pred,const Obj,const Doc) {
      return index.get_range();
   }
};

/**
*******************************************************************************/
template<class QArgs> class Query_by_element {
   typedef typename fusion_rof::as_vector<QArgs>::type store_t;

   struct Compare {
      template<class T> bool operator()(T const& t) const {
         BOOST_MPL_ASSERT_RELATION(mpl::size<T>::value, ==, 2);
         BOOST_MPL_ASSERT((
                  boost::has_equal_to<
                     typename mpl::at_c<T,0>::type,
                     typename mpl::at_c<T,1>::type,
                     bool
                  >
         ));
         return fusion::at_c<0>(t) == fusion::at_c<1>(t);
      }
   };

public:
   template<class T1,class T2, class T3, class T4>
   Query_by_element(T1 const& t1, T2 const& t2, T3 const& t3, T4 const& t4)
   : stor_(t1, t2, t3, t4)
   {}

   bool operator()(Triple const& t) const {
      return fusion::all(fusion::zip(stor_, t), Compare());
   }

private:
   store_t stor_;
};

/** Define number of types in sequence that are not @b any
*******************************************************************************/
template<class QArgs> struct Count_defined {
   typedef typename mpl::fold<
      QArgs,
      mpl::size_t<0>,
      mpl::if_<
         boost::is_same<mpl::_2, any>,
         mpl::_1,
         mpl::next<mpl::_1>
      >
   >::type type;
   static const std::size_t value  = type::value;
};

/**
*******************************************************************************/
template<class QArgs, class Range1, std::size_t N> class Filter_by_element_impl {
   typedef typename boost::range_iterator<Range1>::type iter1;
   typedef Query_by_element<QArgs> query;
public:
   typedef boost::filter_iterator<query, iter1> iterator;
   typedef boost::iterator_range<iterator> range;

   template<class T1,class T2, class T3, class T4> static range get(
            Range1 r, T1 const& t1, T2 const& t2, T3 const& t3, T4 const& t4
   ) {
      query q(t1,t2,t3,t4);
      return boost::make_iterator_range(
               iterator(q, boost::begin(r), boost::end(r)),
               iterator(q, boost::end(r), boost::end(r))
      );
   }

};

/**
*******************************************************************************/
template<class QArgs, class Range1> struct Filter_by_element_impl<QArgs,Range1,0> {
   typedef typename boost::range_iterator<Range1>::type iterator;
   typedef Range1 range;
   template<class T1,class T2, class T3, class T4> static range get(
            Range1 r, T1 const&, T2 const&, T3 const&, T4 const&
   ) {
      return r;
   }
};

/** Filter range of triples by matching each triple's elements
*******************************************************************************/
template<class QArgs, class Range1> class Filter_by_element : public
Filter_by_element_impl<QArgs, Range1, Count_defined<QArgs>::value>
{};

/** Deduce query argument types from booleans
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> class Deduce_args {
   typedef mpl::vector_c<bool,Subj,Pred,Obj,Doc> args_vector;
public:
   typedef typename mpl::fold<
            triple_tags,
            mpl::vector<>,
            mpl::if_<
               mpl::at<args_vector, mpl::_2>,
               mpl::push_back<mpl::_1, mpl::at<Triple, mpl::_2> >,
               mpl::push_back<mpl::_1, any>
            >
   >::type type;
};

/** Define the tag for the best search index, secondary query tags
@tparam SConfig specialized Store_config
@tparam QArgs sequence of query arguments
*******************************************************************************/
template<class SConfig, class QArgs> class Search_config_v {
   typedef typename SConfig::tags s_tags;
   BOOST_MPL_ASSERT_RELATION(mpl::size<s_tags>::value, >, 0);
   BOOST_MPL_ASSERT_RELATION(mpl::size<s_tags>::value, <=, 5);
   BOOST_MPL_ASSERT((boost::is_same<typename mpl::front<s_tags>::type, Main_store_tag>));
   BOOST_MPL_ASSERT_RELATION(mpl::size<QArgs>::value, ==, 4);

   typedef typename Query_tags<QArgs>::type q_tags;

public:
   typedef typename mpl::fold<
            typename mpl::erase<s_tags, typename mpl::begin<s_tags>::type>::type,
            Main_store_tag,
            mpl::if_<
               mpl::contains<q_tags, mpl::_2>,
               mpl::_2,
               mpl::_1
            >
   >::type index_tag;

   typedef typename mpl::distance<
            typename mpl::begin<s_tags>::type,
            typename mpl::find<s_tags, index_tag>::type
   >::type index_num;

   typedef typename mpl::fold<
            triple_tags,
            mpl::vector<>,
            mpl::if_<
               boost::is_same<mpl::_2, index_tag>,
               mpl::push_back<mpl::_1, any>,
               mpl::push_back<mpl::_1, mpl::at<QArgs, mpl::_2> >
            >
   >::type q2_args;

   typedef typename
            fusion_rof::value_at<typename SConfig::store,index_num>::type
            index;
   typedef Filter_by_element<q2_args, typename index::const_range> search;
   typedef typename search::iterator iterator;
   typedef typename search::range range;
};

/**
*******************************************************************************/
template<class SConfig, class Subj, class Pred, class Obj, class Doc>
class Search_config :
   public Search_config_v<SConfig, mpl::vector<Subj,Pred,Obj,Doc> >
{};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_MAP_CONFIG_HPP_ */
