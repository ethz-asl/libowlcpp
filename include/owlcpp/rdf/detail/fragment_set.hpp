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
#include "boost/fusion/sequence/comparison.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/range.hpp"
#include "boost/range/algorithm/equal_range.hpp"
#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{ namespace map_triple_detail{

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
      typedef boost::filter_iterator<
               std::equal_to<value_type, boost::fusion::vector3<U0,U1,U2> >,
               const_iterator
               >
      iterator;
      typedef boost::iterator_range<iterator> range;
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
   typename result<U0,U1,U2>::range
   find(boost::fusion::vector3<U0,U1,U2> const& t) const {
      //todo:
   }

   const_range find(value_type const& t) const {
      return boost::equal_range(v_, t);
   }

   const_range find(boost::fusion::vector3<any,any,any> const& ) const {
      return v_;
   }

private:
   stor_t v_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* FRAGMENT_SET_HPP_ */
