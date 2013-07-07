/** @file "/owlcpp/include/owlcpp/rdf/detail/fragment_set.hpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef FRAGMENT_SET_HPP_
#define FRAGMENT_SET_HPP_
#include <vector>
#include <algorithm>
#include <functional>
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/sequence/comparison/less.hpp"
#include "boost/fusion/sequence/comparison/equal_to.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/range.hpp"
#include "boost/range/algorithm/equal_range.hpp"
#include "boost/type_traits/has_operator.hpp"
#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{ namespace map_triple_detail{

template<
   class T0, class T1, class T2,
   class Q0, class Q1, class Q2
> class Fragment_equal
         : public std::unary_function<boost::fusion::vector3<T0,T1,T2>, bool> {
   typedef boost::fusion::vector3<Q0,Q1,Q2> stor_t;
   typedef boost::fusion::vector3<T0,T1,T2> fragment;

public:
   explicit Fragment_equal(const Q0 u0, const Q1 u1, const Q2 u2) : e_(u0,u1,u2) {}
   bool operator()(fragment const& f) const {return f == e_;}

private:
   stor_t e_;
};

template<
   class T0, class T1, class T2,
   class Q0, class Q1, class Q2
> class Fragment_find_dispatch {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> vector;
   typedef Fragment_equal<T0,T1,T2,Q0,Q1,Q2> equal;
public:
   typedef boost::filter_iterator<equal, typename vector::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;
   static range find(vector const& v, const Q0 u0, const Q1 u1, const Q2 u2) {
      const equal eq(u0,u1,u2);
      return range(
               iterator(eq, v.begin(), v.end()),
               iterator(eq, v.end(), v.end())
      );
   }
};

template<
   class T0, class T1, class T2
> class Fragment_find_dispatch<T0,T1,T2,T0,T1,T2> {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> vector;
public:
   typedef typename vector::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;
   static range find(vector const& v, const T0 t0, const T1 t1, const T2 t2) {
      return boost::equal_range(v, fragment(t0,t1,t2));
   }
};

template<
   class T0, class T1, class T2
> class Fragment_find_dispatch<T0,T1,T2,T0,T1,any> {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> vector;
public:
   typedef typename vector::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;
   static range find(vector const& v, const T0 t0, const T1 t1, const any) {
      return boost::equal_range(
               v,
               boost::fusion::vector3<T0,T1,any>(t0,t1,any())
      );
   }
};

template<
   class T0, class T1, class T2
> class Fragment_find_dispatch<T0,T1,T2,T0,any,any> {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> vector;
public:
   typedef typename vector::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;
   static range find(vector const& v, const T0 t0, const any, const any) {
      return boost::equal_range(
               v,
               boost::fusion::vector3<T0,any,any>(t0, any(), any())
      );
   }
};

template<
   class T0, class T1, class T2
> class Fragment_find_dispatch<T0,T1,T2,any,any,any> {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> vector;
public:
   typedef typename vector::const_iterator iterator;
   typedef boost::iterator_range<iterator> range;
   static range find(vector const& v, const any, const any, const any) {
      return v;
   }
};

/**@brief 
*******************************************************************************/
template<class T0, class T1, class T2> class Fragment_set {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> stor_t;
public:
   typedef fragment value_type;
   typedef typename stor_t::iterator iterator;
   typedef typename stor_t::const_iterator const_iterator;
   typedef boost::iterator_range<const_iterator> const_range;

   template<class Q0, class Q1, class Q2> struct result {
      typedef typename Fragment_find_dispatch<T0,T1,T2,Q0,Q1,Q2>::range type;
   };

   const_iterator begin() const {return v_.begin();}
   const_iterator end() const {return v_.end();}
   std::size_t size() const {return v_.size();}
   bool empty() const {return v_.empty();}

   void insert(value_type const& t) {
      iterator i = std::lower_bound(v_.begin(), v_.end(), t);
      if( i == v_.end() || (t < *i) ) {
         v_.insert(i, t);
      }
   }

   void erase(value_type const& t) {
      iterator i = std::lower_bound(v_.begin(), v_.end(), t);
      if( i == v_.end() || (t < *i) ) BOOST_THROW_EXCEPTION(
               Rdf_err() << Rdf_err::msg_t("triple fragment not found")
      );
      v_.erase(i);
   }

   template<class Q0, class Q1, class Q2>
   typename Fragment_find_dispatch<T0,T1,T2,Q0,Q1,Q2>::range
   find(const Q0 u0, const Q1 u1, const Q2 u2) const {
      BOOST_MPL_ASSERT((boost::has_equal_to<T0,Q0,bool>));
      BOOST_MPL_ASSERT((boost::has_equal_to<T1,Q1,bool>));
      BOOST_MPL_ASSERT((boost::has_equal_to<T2,Q2,bool>));
      return Fragment_find_dispatch<T0,T1,T2,Q0,Q1,Q2>::find(v_, u0, u1, u2);
   }

private:
   stor_t v_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* FRAGMENT_SET_HPP_ */
