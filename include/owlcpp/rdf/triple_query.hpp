/** @file "/owlcpp/include/owlcpp/rdf/triple_query.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_QUERY_HPP_
#define TRIPLE_QUERY_HPP_
#include "boost/iterator/filter_iterator.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/mpl/if.hpp"
#include "boost/type_traits/is_same.hpp"

#include "owlcpp/rdf/triple_map.hpp"

namespace owlcpp{ namespace detail {

template<bool S, bool P, bool O, bool D> struct Triple_bool2type {
   typedef typename boost::mpl::if_<boost::mpl::bool_<S>, Node_id, blank>::type subj_t;
   typedef typename boost::mpl::if_<boost::mpl::bool_<P>, Node_id, blank>::type pred_t;
   typedef typename boost::mpl::if_<boost::mpl::bool_<O>, Node_id, blank>::type obj_t;
   typedef typename boost::mpl::if_<boost::mpl::bool_<D>, Doc_id, blank>::type doc_t;
};

template<class S, class P, class O, class D> struct Query_type {
   static const bool subj = boost::is_same<S, Node_id>::type::value;
   static const bool pred = boost::is_same<P, Node_id>::type::value;
   static const bool obj = boost::is_same<O, Node_id>::type::value;
   static const bool doc = boost::is_same<D, Doc_id>::type::value;
   typedef Query<subj, pred, obj, doc> query_t;
};

template<class,class,class,class> class Triple_predicate;

template<class Pred, class Iter1> struct Selector_2 {
   typedef boost::filter_iterator<Pred, Iter1> iter_t;
};

template<class Iter1> struct Selector_2<Triple_predicate<blank,blank,blank,blank>, Iter1> {
   typedef Iter1 iter_t;
};

/** Check for matching node ID
*******************************************************************************/
template<class Id> class Check {
public:
   explicit Check(const Id id) : id_(id) {}
   bool operator()(const Id id) const {return id == id_;}
private:
   Id id_;
};

template<> struct Check<blank> {
   template<class T> explicit Check(const T) {}
   template<class T> bool operator()(const T) const {return true;}
};

/**
*******************************************************************************/
template<class S, class P, class O, class D> class Triple_predicate {
   typedef boost::tuple<Check<S>,Check<P>,Check<O>,Check<D> > tuple_t;
   typedef Triple_predicate self_t;

   Triple_predicate();

   template<class S1, class P1, class O1, class D1>
   Triple_predicate(const S1 n1, const P1 n2, const O1 n3, const D1 d)
   : t_(Check<S>(n1),Check<P>(n2),Check<O>(n3),Check<D>(d)) {}

public:
   template<class Iter, class S1, class P1, class O1, class D1> static
   typename Selector_2<self_t, Iter>::iter_t
   make_iterator(Iter i1, Iter i2, const S1 s, const P1 p, const O1 o, const D1 d) {
      return typename Selector_2<self_t, Iter>::iter_t(self_t(s,p,o,d), i1, i2);
   }

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

template<> class Triple_predicate<blank,blank,blank,blank> {
   typedef Triple_predicate self_t;
   Triple_predicate();
public:
   template<class Iter, class S1, class P1, class O1, class D1> static
   typename Selector_2<self_t, Iter>::iter_t
   make_iterator(Iter i1, Iter, const S1, const P1, const O1, const D1) {
      return i1;
   }
};

/** Declare and make iterator 1 and declare predicate
*******************************************************************************/
template<class,class,class,class> struct Selector_1;

template<> struct Selector_1<blank,blank,blank,blank> {
   typedef Triple_predicate<blank,blank,blank,blank> predicate_t;
   typedef Triple_map::store_t::iterator iter_t;

   static iter_t begin(Triple_map const& tm, const blank, const blank, const blank, const blank) {
      return tm.store_.begin();
   }

   static iter_t end(Triple_map const& tm, const blank, const blank, const blank, const blank) {
      return tm.store_.end();
   }
};

template<class P, class O, class D> class Selector_1<Node_id,P,O,D> {
   typedef typename Triple_map::subj_tag tag_t;
public:
   typedef Triple_predicate<blank,P,O,D> predicate_t;
   typedef typename Triple_map::store_t::index<tag_t>::type::iterator iter_t;

   static iter_t begin(Triple_map const& tm, const Node_id s, const P, const O, const D) {
      return tm.store_.get<tag_t>().find(s);
   }

   static iter_t end(Triple_map const& tm, const Node_id, const P, const O, const D) {
      return tm.store_.get<tag_t>().end();
   }
};

template<class P, class D> class Selector_1<blank,P,Node_id,D> {
   typedef typename Triple_map::obj_tag tag_t;
public:
   typedef Triple_predicate<blank,P,blank,D> predicate_t;
   typedef typename Triple_map::store_t::index<tag_t>::type::iterator iter_t;

   static iter_t begin(Triple_map const& tm, const blank, const P, const Node_id o, const D) {
      return tm.store_.get<tag_t>().find(o);
   }

   static iter_t end(Triple_map const& tm, const blank, const P, const Node_id, const D) {
      return tm.store_.get<tag_t>().end();
   }
};

template<class D> class Selector_1<blank,Node_id,blank,D> {
   typedef typename Triple_map::pred_tag tag_t;
public:
   typedef Triple_predicate<blank,blank,blank,D> predicate_t;
   typedef typename Triple_map::store_t::index<tag_t>::type::iterator iter_t;

   static iter_t begin(Triple_map const& tm, const blank, const Node_id p, const blank, const D) {
      return tm.store_.get<tag_t>().find(p);
   }

   static iter_t end(Triple_map const& tm, const blank, const Node_id, const blank, const D) {
      return tm.store_.get<tag_t>().end();
   }
};

template<> class Selector_1<blank,blank,blank,Doc_id> {
   typedef typename Triple_map::doc_tag tag_t;
public:
   typedef Triple_predicate<blank,blank,blank,blank> predicate_t;
   typedef typename Triple_map::store_t::index<tag_t>::type::iterator iter_t;

   static iter_t begin(Triple_map const& tm, const blank, const blank, const blank, const Doc_id d) {
      return tm.store_.get<tag_t>().find(d);
   }

   static iter_t end(Triple_map const& tm, const blank, const blank, const blank, const Doc_id) {
      return tm.store_.get<tag_t>().end();
   }
};

}//namespace detail

/**@brief Query triples
*******************************************************************************/
template<bool S = false, bool P = false, bool O = false, bool D = false>
class Query {
   typedef typename detail::Triple_bool2type<S,P,O,D> tts_t;
   typedef typename tts_t::subj_t subj_t;
   typedef typename tts_t::pred_t pred_t;
   typedef typename tts_t::obj_t obj_t;
   typedef typename tts_t::doc_t doc_t;
   typedef typename detail::Selector_1<subj_t, pred_t, obj_t, doc_t> selector1_t;
   typedef typename selector1_t::predicate_t predicate_t;
   typedef typename selector1_t::iter_t iter1_t;
   typedef typename detail::Selector_2<predicate_t, iter1_t> selector2_t;
public:
   typedef typename selector2_t::iter_t iterator_t;
   typedef boost::iterator_range<iterator_t> range_t;

   static range_t
   find(Triple_map const& tm, const subj_t s, const pred_t p, const obj_t o, const doc_t d) {
      const iter1_t i1 = selector1_t::begin(tm, s, p, o, d);
      const iter1_t i2 = selector1_t::end(tm, s, p, o, d);
      return boost::make_iterator_range(
               predicate_t::make_iterator(i1, i2, s, p, o, d),
               predicate_t::make_iterator(i2, i2, s, p, o, d)
      );
   }
};

}//namespace owlcpp
#endif /* TRIPLE_QUERY_HPP_ */
