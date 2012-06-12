/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_map_detail.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_DETAIL_HPP_
#define TRIPLE_MAP_DETAIL_HPP_
#include "boost/fusion/adapted/struct/adapt_struct.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/mpl.hpp"
#include "boost/fusion/include/size.hpp"
#include "boost/fusion/include/push_back.hpp"
#include "boost/fusion/include/value_at.hpp"

#include "boost/iterator/indirect_iterator.hpp"

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
#include "boost/mpl/push_front.hpp"
#include "boost/mpl/size_t.hpp"
#include "boost/mpl/vector.hpp"
#include "boost/mpl/vector_c.hpp"

#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/type_traits/is_same.hpp"

#include "owlcpp/rdf/triple.hpp"

BOOST_FUSION_ADAPT_STRUCT(
         owlcpp::Triple,
         (owlcpp::Node_id, subj_)
         (owlcpp::Node_id, pred_)
         (owlcpp::Node_id, obj_)
         (owlcpp::Doc_id, doc_)
)

namespace owlcpp{

struct any;

namespace detail{
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
namespace fres = boost::fusion::result_of;

/* Tags for query elements
*******************************************************************************/
struct Subj_tag : public mpl::size_t<0> {};
struct Pred_tag : public mpl::size_t<1> {};
struct Obj_tag  : public mpl::size_t<2> {};
struct Doc_tag  : public mpl::size_t<3> {};
typedef mpl::vector<Subj_tag, Pred_tag, Obj_tag, Doc_tag> triple_tags;

/**
*******************************************************************************/
struct Element_index {
   typedef std::vector<Triple const*> type;
   typedef boost::indirect_iterator<type::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;
};

/**
*******************************************************************************/
template<class Id> class Vector_index {
   typedef std::vector<Element_index::type> stor_t;
public:

   void add(const Id id, Triple const& t) {
      if( id() >= stor_.size() ) stor_.resize(id() + 1);
      stor_[id()].push_back(&t);
   }

private:
   stor_t stor_;
};

/**
*******************************************************************************/
template<class Id> class Map_index {
   typedef std::map<Id, Element_index::type> stor_t;
public:
   void add(const Id id, Triple const& t) {
      stor_[id].push_back(&t);
   }

private:
   stor_t stor_;
};

/**
*******************************************************************************/
template<class Tag> struct Index
: private Vector_index<typename fres::value_at<Triple,Tag>::type> {
   void add(Triple const& t) {
      Vector_index<typename fres::value_at<Triple,Tag>::type>::add(fusion::at<Tag>(t), t);
   }
};

struct Main_store_tag{};

/**
*******************************************************************************/
template<> struct Index<Main_store_tag> {
   typedef Element_index::type stor_t;
public:
   void add(Triple const& t) {
      stor_.push_back(&t);
   }

private:
   stor_t stor_;
};

template<> struct Index<Pred_tag>
: private Map_index<fres::value_at<Triple,Pred_tag>::type> {
   void add(Triple const& t) {
      Map_index<fres::value_at<Triple,Pred_tag>::type>::add(fusion::at<Pred_tag>(t), t);
   }
};

/** Make a vector of tags for each index to be created, including the tag for
the main store of triples.
The tags are listed in the order of increasing (projected) search performance.
For example, subject index is expected to be best performing and the possibility
for using it should be checked first; the main store is expected to have the worst
performance.
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> struct Index_types {
   typedef mpl::vector_c<bool,Subj,Pred,Obj,Doc> args_vector;
   //tags in the order of priority
   typedef mpl::vector<Doc_tag, Pred_tag, Obj_tag, Subj_tag> priority_tags;
   typedef typename mpl::fold<
            priority_tags,
            mpl::vector<>,
            mpl::if_<
               mpl::at<args_vector, mpl::_2>,
               mpl::push_back<mpl::_1, mpl::_2>,
               mpl::_1
            >
   >::type index_tags;

   typedef typename mpl::push_front<index_tags, Main_store_tag>::type store_tags;

   typedef typename mpl::fold<
            store_tags,
            fusion::vector<>,
            mpl::push_back<mpl::_1, Index<mpl::_2> >
   >::type store_type;
};

/**
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc> struct Query_ft {
   typedef mpl::vector<Subj,Pred,Obj,Doc> args_vector;
   typedef typename mpl::fold<
            triple_tags,
            mpl::vector<>,
            mpl::if_<
               boost::is_same<mpl::at<args_vector, mpl::_2>, any>,
               mpl::_1,
               mpl::push_back<mpl::_1, mpl::_2>
            >
   >::type tags;
};

/**
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> struct Query_fb {
   typedef mpl::vector_c<bool,Subj,Pred,Obj,Doc> args_vector;
   typedef typename mpl::fold<
            triple_tags,
            mpl::vector<>,
            mpl::if_<
               mpl::at<args_vector, mpl::_2>,
               mpl::push_back<mpl::_1, mpl::_2>,
               mpl::_1
            >
   >::type tags;
};

/** Define the tag for the best search index, secondary query tags
@targ Stags sequence of store tags in the order of increasing priority,
starting from Main_store_tag
@tags Qtags sequence of tags for the arguments specified in the query
*******************************************************************************/
template<class Stags, class Qtags> class Search_types {
   BOOST_MPL_ASSERT_RELATION(mpl::size<Stags>::value, >, 0);
   BOOST_MPL_ASSERT_RELATION(mpl::size<Stags>::value, <=, 5);
   BOOST_MPL_ASSERT((boost::is_same<typename mpl::front<Stags>::type, Main_store_tag>));
   BOOST_MPL_ASSERT_RELATION(mpl::size<Qtags>::value, <=, 4);
public:
   typedef typename mpl::fold<
            typename mpl::erase<Stags, typename mpl::begin<Stags>::type>::type,
            Main_store_tag,
            mpl::if_<
               mpl::contains<Qtags, mpl::_2>,
               mpl::_2,
               mpl::_1
            >
   >::type index_tag;

   typedef typename mpl::distance<
            typename mpl::begin<Stags>::type,
            typename mpl::find<Stags, index_tag>::type
   >::type index_num;

   typedef typename mpl::eval_if<
            boost::is_same<index_tag, Main_store_tag>,
            mpl::identity<Qtags>,
            mpl::erase<
               Qtags,
               typename mpl::find<Qtags,index_tag>::type
            >
   >::type query_second;
};


/**@brief 
*******************************************************************************/

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_MAP_DETAIL_HPP_ */
