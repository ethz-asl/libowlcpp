/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_set.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_SET_HPP_
#define TRIPLE_SET_HPP_
#include <algorithm>
#include <functional>
#include <vector>
#include "boost/assert.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/mpl/at.hpp"
#include "boost/range.hpp"
#include "boost/range/algorithm_ext/is_sorted.hpp"
#include "boost/range/algorithm/equal_range.hpp"
#include "boost/range/algorithm/lower_bound.hpp"
#include "boost/range/algorithm/upper_bound.hpp"
#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/detail/adapt_triple.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Compare triples with triples or with query elements
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3,
   class Q1 = typename boost::mpl::at<Triple, Tag1>::type,
   class Q2 = typename boost::mpl::at<Triple, Tag2>::type,
   class Q3 = typename boost::mpl::at<Triple, Tag3>::type
> struct Value_predicate : public std::unary_function<Triple, bool> {
public:
   Value_predicate() {}

   Value_predicate(const Q1 q1, const Q2 q2, const Q3 q3)
   : q1_(q1), q2_(q2), q3_(q3)
   {}

   explicit Value_predicate(Triple const& t)
   : q1_(boost::fusion::at<Tag1>(t)),
     q2_(boost::fusion::at<Tag2>(t)),
     q3_(boost::fusion::at<Tag3>(t))
   {}

   bool operator() (Triple const& t) const {return t == *this;}

   bool operator() (Triple const& t1, Triple const& t2) const {
      using boost::fusion::at;
      if( at<Tag1>(t1) < at<Tag1>(t2) ) return true;
      if( at<Tag1>(t2) < at<Tag1>(t1) ) return false;
      if( at<Tag2>(t1) < at<Tag2>(t2) ) return true;
      if( at<Tag2>(t2) < at<Tag2>(t1) ) return false;
      return at<Tag3>(t1) < at<Tag3>(t2);
   }

   friend bool operator<(Triple const& t, Value_predicate const& vp) {
      using boost::fusion::at;
      if( at<Tag1>(t)  < vp.q1_       ) return true;
      if( vp.q1_        < at<Tag1>(t) ) return false;
      if( at<Tag2>(t)  < vp.q2_       ) return true;
      if( vp.q2_        < at<Tag2>(t) ) return false;
      return at<Tag3>(t) < vp.q3_;
   }

   friend bool operator<(Value_predicate const& vp, Triple const& t) {
      using boost::fusion::at;
      if( vp.q1_        < at<Tag1>(t)  ) return true;
      if( at<Tag1>(t)   < vp.q1_       ) return false;
      if( vp.q2_        < at<Tag2>(t)  ) return true;
      if( at<Tag2>(t)   < vp.q2_       ) return false;
      return vp.q3_ < at<Tag3>(t);
   }

   friend bool operator==(Triple const& t1, Value_predicate const& vp) {
      using boost::fusion::at;
      return
               at<Tag1>(t1) == vp.q1_ &&
               at<Tag2>(t1) == vp.q2_ &&
               at<Tag3>(t1) == vp.q3_
               ;
   }

private:
   Q1 q1_;
   Q2 q2_;
   Q3 q3_;
};

/**@brief Define elements of Triple_set used by other classes
*******************************************************************************/
template<class Tag1, class Tag2, class Tag3> struct Triple_set_config {
   typedef std::vector<Triple> storage;
   typedef Triple value_type;
   typedef storage::iterator iterator;
   typedef storage::const_iterator const_iterator;
   typedef boost::iterator_range<const_iterator> range;
};

/**@brief Find elements in a sorted range
@details Find elements by sequential search
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3,
   class Q1, class Q2, class Q3
> class Ts_query_dispatch {
   typedef Value_predicate<Tag1,Tag2,Tag3,Q1,Q2,Q3> predicate;
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
   typedef typename config::const_iterator iter;
public:
   typedef boost::filter_iterator<predicate, iter> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Q1 q1, const Q2 q2, const Q3 q3) {
      const predicate p(q1,q2,q3);
      return range(
               iterator(p, v.begin(), v.end()),
               iterator(p, v.end(), v.end())
      );
   }

   static const int efficiency = 0;
};

/**@brief Find elements in a sorted range
@details Return the whole range
*******************************************************************************/
template<class Tag1, class Tag2, class Tag3>
class Ts_query_dispatch<Tag1,Tag2,Tag3,Any,Any,Any> {
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;

public:
   typedef typename config::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Any, const Any, const Any) {
      return range(v);
   }

   static const int efficiency = 0;
};

/**@brief Find elements in a sorted range
@details Find elements by sorted range search followed by sequential search
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3,
   class Q2, class Q3
> class Ts_query_dispatch<
   Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag1>::type,
   Q2, Q3
> {
   typedef typename boost::mpl::at<Triple, Tag1>::type Q1;
   typedef Value_predicate<Tag1,Tag2,Tag3,Q1,Any,Any> pred1;
   typedef Value_predicate<Tag1,Tag2,Tag3,Any,Q2,Q3> pred2;
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
   typedef typename config::const_iterator iter1;
   typedef boost::iterator_range<iter1> range1;
public:
   typedef boost::filter_iterator<pred2, iter1> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Q1 q1, const Q2 q2, const Q3 q3) {
      const pred1 p1(q1, any, any);
      BOOST_ASSERT(boost::is_sorted(v, p1));
      range1 r = boost::equal_range(v, p1);
      const pred2 p2(any,q2,q3);
      return range(
               iterator(p2, r.begin(), r.end()),
               iterator(p2, r.end(),   r.end())
      );
   }

   static const int efficiency = 10;
};

/**@brief Find elements in a sorted range
@details Find elements by sorted range search
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3
> class Ts_query_dispatch<
   Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag1>::type,
   Any, Any
> {
   typedef typename boost::mpl::at<Triple, Tag1>::type Q1;
   typedef Value_predicate<Tag1,Tag2,Tag3,Q1,Any,Any> pred;
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
public:
   typedef typename config::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Q1 q1, const Any, const Any) {
      const pred p(q1, any, any);
      BOOST_ASSERT(boost::is_sorted(v, p));
      return boost::equal_range(v, p);
   }

   static const int efficiency = 10;
};

/**@brief Find elements in a sorted range
@details Find elements by sorted range search followed by sequential search
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3,
   class Q3
> class Ts_query_dispatch<
   Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag1>::type,
   typename boost::mpl::at<Triple, Tag2>::type,
   Q3
> {
   typedef typename boost::mpl::at<Triple, Tag1>::type Q1;
   typedef typename boost::mpl::at<Triple, Tag2>::type Q2;
   typedef Value_predicate<Tag1,Tag2,Tag3,Q1,Q2,Any> pred1;
   typedef Value_predicate<Tag1,Tag2,Tag3,Any,Any,Q3> pred2;
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
   typedef typename config::const_iterator iter1;
   typedef boost::iterator_range<iter1> range1;
public:
   typedef boost::filter_iterator<pred2, iter1> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Q1 q1, const Q2 q2, const Q3 q3) {
      const pred1 p1(q1, q2, any);
      BOOST_ASSERT(boost::is_sorted(v, p1));
      range1 r = boost::equal_range(v, p1);
      pred2 p2(any,any,q3);
      return range(
               iterator(p2, r.begin(), r.end()),
               iterator(p2, r.end(),   r.end())
      );
   }

   static const int efficiency = 15;
};

/**@brief Find elements in a sorted range
@details Find elements by sorted range search
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3
> class Ts_query_dispatch<
   Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag1>::type,
   typename boost::mpl::at<Triple, Tag2>::type,
   Any
> {
   typedef typename boost::mpl::at<Triple, Tag1>::type Q1;
   typedef typename boost::mpl::at<Triple, Tag2>::type Q2;
   typedef Value_predicate<Tag1,Tag2,Tag3,Q1,Q2,Any> pred1;
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
public:
   typedef typename config::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Q1 q1, const Q2 q2, const Any) {
      const pred1 p1(q1, q2, any);
      BOOST_ASSERT(boost::is_sorted(v, p1));
      return boost::equal_range(v, p1);
   }

   static const int efficiency = 15;
};

/**@brief Find elements in a sorted range
@details Find elements by sorted range search
*******************************************************************************/
template<
   class Tag1, class Tag2, class Tag3
> class Ts_query_dispatch<
   Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag1>::type,
   typename boost::mpl::at<Triple, Tag2>::type,
   typename boost::mpl::at<Triple, Tag3>::type
> {
   typedef typename boost::mpl::at<Triple, Tag1>::type Q1;
   typedef typename boost::mpl::at<Triple, Tag2>::type Q2;
   typedef typename boost::mpl::at<Triple, Tag3>::type Q3;
   typedef Value_predicate<Tag1,Tag2,Tag3,Q1,Q2,Q3> pred1;
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
public:
   typedef typename config::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& v, const Q1 q1, const Q2 q2, const Q3 q3) {
      const pred1 p1(q1, q2, q3);
      BOOST_ASSERT(boost::is_sorted(v, p1));
      return boost::equal_range(v, p1);
   }

   static const int efficiency = 20;
};

/**@brief
*******************************************************************************/
template<class Tag1, class Tag2, class Tag3> class Triple_set {
   typedef Triple_set_config<Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;

public:
   typedef Triple value_type;
   typedef typename config::iterator iterator;
   typedef typename config::const_iterator const_iterator;
   typedef boost::iterator_range<const_iterator> range;

   static Triple_set const& empty_set() {
      static const Triple_set s;
      return s;
   }

   Triple_set() {}

   const_iterator begin() const {return v_.begin();}
   const_iterator end() const {return v_.end();}
   std::size_t size() const {return v_.size();}
   bool empty() const {return v_.empty();}

   /** for testing only */
   Triple const& operator[](const std::size_t n) const {return v_[n];}

   bool insert(Triple const& t) {
      Value_predicate<Tag1,Tag2,Tag3> vp(t);
      iterator i = boost::lower_bound(v_, vp);
      if( i == v_.end() || !(*i == vp) ) {
         v_.insert(i, t);
         return true;
      }
      return false;
   }

   void erase(Triple const& t) {
      Value_predicate<Tag1,Tag2,Tag3> vp(t);
      iterator i = boost::lower_bound(v_, vp);
      if( i == v_.end() || !(*i == vp) ) BOOST_THROW_EXCEPTION(
               Rdf_err() << Rdf_err::msg_t("triple fragment not found")
      );
      v_.erase(i);
   }

   template<class Q1, class Q2, class Q3> struct query {
      typedef Ts_query_dispatch<Tag1,Tag2,Tag3,Q1,Q2,Q3> dispatch;
      typedef typename dispatch::iterator iterator;
      typedef typename dispatch::range range;
      static const int efficiency = dispatch::efficiency;
   };

   template<class Q1, class Q2, class Q3>
   typename query<Q1,Q2,Q3>::range
   find(const Q1 q1, const Q2 q2, const Q3 q3) const {
      return query<Q1,Q2,Q3>::dispatch::find(v_, q1, q2, q3);
   }

private:
   storage v_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_SET_HPP_ */
