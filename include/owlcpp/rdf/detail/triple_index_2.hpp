/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_2.hpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_2_HPP_
#define TRIPLE_INDEX_2_HPP_
#include "boost/foreach.hpp"
#include "boost/fusion/sequence/intrinsic/at_c.hpp"
#include "boost/iterator/iterator_facade.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/contains.hpp"
#include "boost/mpl/remove.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/mpl/sort.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/detail/triple_index_config.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief
*******************************************************************************/
template<
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Iter
> class Triple_iterator
         : public boost::iterator_facade<
           Triple_iterator<Tag0,Tag1,Tag2,Tag3,Iter>,
           Triple,
           boost::use_default,
           Triple
           > {
   typedef Triple_index_config<Tag0,Tag1,Tag2,Tag3> config;
public:
   typedef Iter base_iterator;

   Triple_iterator(const typename config::el0 el0, const base_iterator i)
   : el0_(el0), i_(i)
   {}

private:
   typename config::el0 el0_;
   base_iterator i_;

   friend class boost::iterator_core_access;

   void increment() {++i_;}

   bool equal(Triple_iterator const& i) const {
      return el0_ == i.el0_ && i_ == i.i_;
   }

   Triple dereference() const {
      Triple t;
      typename config::fragment const& f = *i_;
      boost::fusion::at<Tag0>(t) = el0_;
      boost::fusion::at<Tag1>(t) = boost::fusion::at_c<1>(f);
      boost::fusion::at<Tag2>(t) = boost::fusion::at_c<2>(f);
      boost::fusion::at<Tag3>(t) = boost::fusion::at_c<3>(f);
      return t;
   }
};

/**@brief Invoke appropriate search algorithm
@tparam Tag0 numerical tag indicating which triple element is indexed first
@tparam Tag1 numerical tag indicating which triple element is indexed second
@tparam Tag2 numerical tag indicating which triple element is indexed third
@tparam Tag3 numerical tag indicating which triple element is indexed fourth
@tparam Q0 type of first-indexed query element
@tparam Q1 type of second-indexed query element
@tparam Q2 type of third-indexed query element
@tparam Q3 type of fourth-indexed query element
*******************************************************************************/
template<
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Q0, class Q1, class Q2, class Q3
> class Triple_find_dispatch;

template<
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Q1, class Q2, class Q3
> class Triple_find_dispatch<
   Tag0,Tag1,Tag2,Tag3,typename boost::mpl::at<Triple, Tag0>::type,Q1,Q2,Q3
> {
   typedef Triple_index_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::fragment_set fragment_set;
   typedef typename config::storage storage;
   typedef typename config::el1 el1;
   typedef typename config::el2 el2;
   typedef typename config::el3 el3;
   typedef typename Fragment_find_dispatch<el1,el2,el3,Q1,Q2,Q3>::iterator
            fragment_iter;

public:
   typedef Triple_iterator<Tag0,Tag1,Tag2,Tag3,fragment_iter> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(
            storage const& v,
            typename boost::mpl::at<Triple, Tag0>::type const q0,
            Q1 const& q1,
            Q2 const& q2,
            Q3 const& q3
   ) {

   }
};

template<class Tag0, class Tag1, class Tag2, class Tag3>
class Triple_find_dispatch<
   Tag0,Tag1,Tag2,Tag3,typename boost::mpl::at<Triple, Tag0>::type,any,any,any
> {
   typedef Triple_index_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::fragment_set fragment_set;
   typedef typename config::storage storage;
   typedef typename config::el1 el1;
   typedef typename config::el2 el2;
   typedef typename config::el3 el3;
   typedef typename fragment_set::const_iterator fragment_iter;
public:
   typedef Triple_iterator<Tag0,Tag1,Tag2,Tag3,fragment_iter> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(
            storage const& v,
            typename boost::mpl::at<Triple, Tag0>::type const q0,
            any const& = any(),
            any const& = any(),
            any const& = any()
   ) {
      v[q0()]
   }

};

/**@brief 
*******************************************************************************/
template<
   template<class,class,class,class> class Map,
   class Tag0,
   class Tag1,
   class Tag2,
   class Tag3
> class Triple_index {
   typedef boost::mpl::vector4<Tag0,Tag1,Tag2,Tag3> tags;
   BOOST_MPL_ASSERT((
            boost::mpl::equal<typename boost::mpl::sort<tags>::type, triple_tags>
   ));
   typedef Triple_index_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::el0 el0;
   typedef typename config::el1 el1;
   typedef typename config::el2 el2;
   typedef typename config::el3 el3;
   typedef typename config::fragment fragment;
   typedef typename config::fragment_set fragment_set;
   typedef typename config::storage storage;

public:
/*
   typedef typename
         Triple_find_dispatch<Tag0,Tag1,Tag2,Tag3,any,any,any,any>::iterator
         iterator;
   typedef iterator const_iterator;

   const_iterator begin() const {
      return Triple_find_dispatch<
                  Tag0,Tag1,Tag2,Tag3,any,any,any,any
            >::begin(v_);
   }

   const_iterator end() const {
      return Triple_find_dispatch<
                  Tag0,Tag1,Tag2,Tag3,any,any,any,any
            >::end(v_);
   }
*/

   void insert(Triple const& t) {
      const std::size_t n = boost::fusion::at<Tag0>(t)();
      if( n >= v_.size() ) v_.resize(n + 1);
      v_[n].insert(
               fragment(
                        boost::fusion::at<Tag1>(t),
                        boost::fusion::at<Tag2>(t),
                        boost::fusion::at<Tag3>(t)
               )
      );
   }

   std::size_t size() const {
      std::size_t n = 0;
      BOOST_FOREACH(fragment_set const& fs, v_) {
         n += fs.size();
      }
      return n;
   }

   bool empty() const {
      BOOST_FOREACH(fragment_set const& fs, v_) {
         if( ! fs.empty() ) return false;
      }
      return true;
   }

   template<class Q0, class Q1, class Q2, class Q3>
   typename Triple_find_dispatch<Tag0,Tag1,Tag2,Tag3,Q0,Q1,Q2,Q3>::range
   find(Q0 const& q0, Q1 const& q1, Q2 const& q2, Q3 const& q3) const {
      typedef boost::mpl::vector4<Q0,Q1,Q2,Q3> qt;
      typedef typename boost::mpl::at<qt, Tag0>::type qt0;
      typedef typename boost::mpl::at<qt, Tag1>::type qt1;
      typedef typename boost::mpl::at<qt, Tag2>::type qt2;
      typedef typename boost::mpl::at<qt, Tag3>::type qt3;

      BOOST_MPL_ASSERT((boost::has_equal_to<el0,qt0,bool>));
      BOOST_MPL_ASSERT((boost::has_equal_to<el1,qt1,bool>));
      BOOST_MPL_ASSERT((boost::has_equal_to<el2,qt2,bool>));
      BOOST_MPL_ASSERT((boost::has_equal_to<el3,qt3,bool>));
      const boost::fusion::vector4<Q0,Q1,Q2,Q3> q(q0,q1,q2,q3);
      return Triple_find_dispatch<Tag0,Tag1,Tag2,Tag3,qt0,qt1,qt2,qt3>::find(
            v_,
            boost::fusion::at<Tag0>(q),
            boost::fusion::at<Tag1>(q),
            boost::fusion::at<Tag2>(q),
            boost::fusion::at<Tag3>(q)
      );
   }

private:
   storage v_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_2_HPP_ */
