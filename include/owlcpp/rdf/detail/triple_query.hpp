/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_query.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_QUERY_HPP_
#define TRIPLE_QUERY_HPP_

namespace owlcpp{

struct any{};

namespace query_detail{

/**@brief 
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc> class Query_type {

};

/**
*******************************************************************************/
template<> struct Query_type<any,any,any,any> {
   typedef triple_map_store_t::iterator iterator_t;
   typedef boost::iterator_range<iterator_t> range_t;
   static range_t range(
            triple_map_store_t const& tms, const any,
            const any, const any, const any
            ) {
      return boost::make_iterator_range( tms.begin(), tms.end() );
   }
};


}//namespace query_detail


/**@brief
*******************************************************************************/
template<bool S, bool P, bool O, bool D> class Query {
   typedef typename boost::mpl::if_<boost::mpl::bool_<S>, Node_id, any>::type subj_t;
   typedef typename boost::mpl::if_<boost::mpl::bool_<P>, Node_id, any>::type pred_t;
   typedef typename boost::mpl::if_<boost::mpl::bool_<O>, Node_id, any>::type obj_t;
   typedef typename boost::mpl::if_<boost::mpl::bool_<D>, Doc_id, any>::type doc_t;
   typedef typename query_detail::Query_type<subj_t, pred_t, obj_t, doc_t> query_t;
public:
   typedef typename query_t::iterator_t iterator;
   typedef typename query_t::range_t range;
};

}//namespace owlcpp
#endif /* TRIPLE_QUERY_HPP_ */
