/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_query.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_QUERY_HPP_
#define TRIPLE_QUERY_HPP_
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/adapted/mpl.hpp"
#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/fold.hpp"
#include "boost/fusion/include/nview.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/mpl/and.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/contains.hpp"
#include "boost/mpl/erase.hpp"
#include "boost/mpl/find.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/vector.hpp"
#include "boost/mpl/vector_c.hpp"
#include "boost/mpl/size.hpp"
#include "boost/range.hpp"
#include "boost/type_traits/is_same.hpp"

#include "owlcpp/rdf/detail/triple_query_tags.hpp"
#include "owlcpp/rdf/detail/adapt_triple_fusion.hpp"

namespace owlcpp{

struct any{};

namespace query_detail{
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
namespace fres_of = boost::fusion::result_of;

/**@brief
*******************************************************************************/
template<class Arg_tags> class Check_triple {
   typedef typename mpl::fold<
            Arg_tags,
            fusion::vector<>,
            mpl::push_back<mpl::_1, mpl::at<Triple, mpl::_2> >
   >::type args_t;

public:
   template<class Args> explicit Check_triple(Args const& args)
   : args_(fusion::nview<Args, Arg_tags>(args)) {}

private:
   const args_t args_;
};

/**@brief
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc, class Store> class Query_impl {
   typedef mpl::vector<Subj_tag, Pred_tag, Obj_tag, Doc_tag> tag_vector;
   typedef fusion::vector<Subj,Pred,Obj,Doc> arguments;

   typedef typename mpl::fold<
            tag_vector,
            mpl::vector<>,
            mpl::if_<
               boost::is_same<mpl::at<arguments, mpl::_2>, any>,
               mpl::_1,
               mpl::push_back<mpl::_1, mpl::_2>
            >
   >::type arg_tags;

   typedef typename Store::indexed_tags indexed_tags; //vector of indexed tags

   typedef typename mpl::fold<
            indexed_tags,
            any,
            mpl::if_<
               mpl::and_<
                  boost::is_same<mpl::_1, any>,
                  mpl::contains<arg_tags, mpl::_2>
               >,
               mpl::_2,
               mpl::_1
            >
   >::type index_arg_tag; //tag for the optimal index to use
   typedef typename Store::template index<index_arg_tag>::type index_t;
   typedef typename mpl::if_<
            boost::is_same<index_arg_tag, any>,
            arg_tags,
            mpl::erase<arg_tags, mpl::find<arg_tags, index_arg_tag> >
   >::type search_arg_tags;

   typedef Check_triple<search_arg_tags> check_triple_t;

public:
   typedef typename Store::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range make_range(Store const& store,
            const Subj subj, const Pred pred, const Obj obj, const Doc doc
   ) {
      const arguments args(subj, pred, obj, doc);
      const typename index_t::range r1 = index_t::get_range(store, args);
   }
};

/**@brief
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc, class Store> class Query {
   typedef Query_impl<
            typename mpl::if_c<Subj,typename mpl::at_c<Triple,0>::type, any>::type,
            typename mpl::if_c<Pred,typename mpl::at_c<Triple,1>::type, any>::type,
            typename mpl::if_c<Obj, typename mpl::at_c<Triple,2>::type, any>::type,
            typename mpl::if_c<Doc, typename mpl::at_c<Triple,3>::type, any>::type,
            Store
            > query_t;
public:
   typedef typename query_t::iterator iterator;
   typedef typename query_t::range range;
};


}//namespace query_detail


/**@brief
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc, class Store>
class Query_type {
   typedef query_detail::Query_impl<Subj,Pred,Obj,Doc,Store> query_t;
public:
   typedef typename query_t::iterator iterator;
   typedef typename query_t::range range;
};

using query_detail::Query;

}//namespace owlcpp
#endif /* TRIPLE_QUERY_HPP_ */
