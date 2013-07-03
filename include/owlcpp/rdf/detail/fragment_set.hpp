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
#include "boost/range.hpp"
#include "boost/range/algorithm/equal_range.hpp"
#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{ namespace map_triple_detail{

template<
   class T0, class T1, class T2,
   class U0, class U1, class U2
> class Equal
         : public std::unary_function<boost::fusion::vector3<T0,T1,T2>, bool> {
   typedef boost::fusion::vector3<U0,U1,U2> stor_t;
   typedef boost::fusion::vector3<T0,T1,T2> fragment;

public:
   explicit Equal(const U0 u0, const U1 u1, const U2 u2) : e_(u0,u1,u2) {}
   bool operator()(fragment const& f) const {return f == e_;}

private:
   stor_t e_;
};

template<
   class T0, class T1, class T2,
   class U0, class U1, class U2
> class Find {
   typedef boost::fusion::vector3<T0,T1,T2> fragment;
   typedef std::vector<fragment> vector;
   typedef Equal<T0,T1,T2,U0,U1,U2> equal;
public:
   typedef boost::filter_iterator<equal, typename vector::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;
   static range find(vector const& v, const U0 u0, const U1 u1, const U2 u2) {
      const equal eq(u0,u1,u2);
      return range(
               iterator(eq, v.begin(), v.end()),
               iterator(eq, v.end(), v.end())
      );
   }
};

template<
   class T0, class T1, class T2
> class Find<T0,T1,T2,T0,T1,T2> {
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
> class Find<T0,T1,T2,T0,T1,any> {
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
> class Find<T0,T1,T2,T0,any,any> {
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
> class Find<T0,T1,T2,any,any,any> {
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

   template<class U0, class U1, class U2> struct result {
      typedef typename Find<T0,T1,T2,U0,U1,U2>::range type;
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

   template<class U0, class U1, class U2>
   typename Find<T0,T1,T2,U0,U1,U2>::range
   find(const U0 u0, const U1 u1, const U2 u2) const {
      return Find<T0,T1,T2,U0,U1,U2>::find(v_, u0, u1, u2);
   }

private:
   stor_t v_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* FRAGMENT_SET_HPP_ */
