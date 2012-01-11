/** @file "/owlcpp/include/owlcpp/rdf/triple_query.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_QUERY_HPP_
#define TRIPLE_QUERY_HPP_
#include "boost/iterator/filter_iterator.hpp"
#include "boost/tuple/tuple.hpp"

#include "owlcpp/rdf/triple_map.hpp"

namespace owlcpp{ namespace detail {

/** Check for matching node ID
*******************************************************************************/
template<class Id> class Check {
public:
   Check(const Id id) : id_(id) {}
   bool operator()(const Id id) const {return id == id_;}
private:
   Id id_;
};

template<> struct Check<blank> {
   Check(const blank) {}
   bool operator()(const Node_id) const {return true;}
};

/**
*******************************************************************************/
template<class S, class P, class O, class D> class Triple_predicate {
   typedef boost::tuple<Check<S>,Check<P>,Check<O>,Check<D> > tuple_t;
public:
   Triple_predicate(const S n1, const P n2, const O n3, const D d)
   : t_(Check<S>(n1),Check<P>(n2),Check<O>(n3),Check<D>(d)) {}

   bool operator()(const Triple& triple) const {
      return
            t_.template get<0>()(triple.subject    ()) &&
            t_.template get<1>()(triple.predicate  ()) &&
            t_.template get<2>()(triple.object     ()) &&
            t_.template get<3>()(triple.document   ())
            ;
   }
private:
   tuple_t t_;
};

/** Tell Query which tag to use for index extraction
*******************************************************************************/
template<class,class,class,class> struct Select_iter1;

template<> struct Select_iter1<blank,blank,blank,blank> {
   typedef typename Triple_map::seq_tag tag_t;
};

template<class P, class O, class D> struct Select_iter1<Node_id,P,O,D> {
   typedef typename Triple_map::subj_tag tag_t;
   typedef Triple_predicate<blank,P,O,D> predicate_t;
};

template<class P, class D> struct Select_iter1<blank,P,Node_id,D> {
   typedef typename Triple_map::obj_tag tag_t;
   typedef Triple_predicate<blank,P,blank,D> predicate_t;
};

template<> struct Select_iter1<blank,Node_id,blank,Doc_id> {
   typedef typename Triple_map::pred_tag tag_t;
   typedef Triple_predicate<blank,blank,blank,Doc_id> predicate_t;
};

template<class S, class P, class O, class D> struct Filter_selector {
   static const bool use_filter = true;
};
template<> struct Filter_selector<blank,blank,blank,blank> {
   static const bool use_filter = false;
};
template<> struct Filter_selector<Node_id,blank,blank,blank> {
   static const bool use_filter = false;
};
template<> struct Filter_selector<blank,Node_id,blank,blank> {
   static const bool use_filter = false;
};
template<> struct Filter_selector<blank,blank,Node_id,blank> {
   static const bool use_filter = false;
};
template<> struct Filter_selector<blank,blank,blank,Node_id> {
   static const bool use_filter = false;
};

template<class, bool> struct Iterator_selector;
template<class Iter1> struct Iterator_selector<Iter1,false> {
   typedef Iter1 iter_t;

//   template<class S, class P, class O, class D> static
};
template<class Tag> struct Iterator_selector<Tag,true> {
   typedef typename Triple_map::store_t::index<Tag>::type index_t;
//   template<class S, class P, class O, class D> static
};

}//namespace detail

/**@brief
*******************************************************************************/
template<class S = blank, class P = blank, class O = blank, class D = blank>
class Query {
   friend class Triple_map;
   typedef Triple_map::store_t store_t;
   typedef typename detail::Select_iter1<S,P,O,D>::tag_t tag_t;
   typedef typename Triple_map::store_t::index<tag_t>::type index_t;
   typedef typename index_t::iterator iter1_t;
   static const bool use_filter = detail::Filter_selector<S,P,O,D>::use_filter;
public:

};

/*
template<> class Query<blank,blank,blank,blank> {
   friend class Triple_map;
   typedef Triple_map::store_t store_t;
public:
   typedef typename store_t::iterator iter_t;
   typedef boost::iterator_range<iter_t> range_t;

private:
   static range_t find(store_t const& stor, const blank, const blank, const blank, const blank){
      return boost::make_iterator_range(stor);
   }
};

template<class P, class O, class D> class Query<Node_id,P,O,D> {
   friend class Triple_map;
   typedef Triple_map::store_t store_t;
public:
   typedef typename store_t::iterator iter_t;
   typedef boost::iterator_range<iter_t> range_t;

private:
   static range_t find(store_t const& stor, const Node_id s, const P p, const O o, const D d){
      typedef typename detail::Select_iter1<Node_id,P,O,D>::tag_t tag_t;
//      typedef boost::filter_iterator<
      return boost::make_iterator_range(stor);
   }
};
*/


/**@brief 
*******************************************************************************/


}//namespace owlcpp
#endif /* TRIPLE_QUERY_HPP_ */
