/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_2.hpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_2_HPP_
#define TRIPLE_INDEX_2_HPP_
#include "boost/foreach.hpp"
#include "boost/iterator/iterator_facade.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/contains.hpp"
#include "boost/mpl/remove.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/mpl/sort.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/detail/convert_fragment.hpp"

namespace owlcpp{ namespace map_triple_detail{

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
   template<class,class> class Map,
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Q0, class Q1, class Q2, class Q3
> class Triple_find_dispatch;

/**@brief
*******************************************************************************/
template<
   class Converter,
   class Iter,
   class Q0, class Q1, class Q2, class Q3
> class Triple_iterator
         : public boost::iterator_facade<
              Triple_iterator<Converter, Iter, Q0, Q1, Q2, Q3>,
              Triple,
              boost::forward_traversal_tag,
              Triple
           > {


   Triple_iterator(const Iter begin, const Iter end,
            Q0 const& q0, Q1 const& q1, Q2 const& q2, Q3 const& q3)
   : begin_(begin),
     end_(end)
     {

     }

   Iter begin_;
   Iter end_;

   friend class boost::iterator_core_access;
   template<
      template<class,class> class Map,
      class Tag0, class Tag1, class Tag2, class Tag3,
      class Q0, class Q1, class Q2, class Q3
   > friend class Triple_find_dispatch;

   void increment() {
      ++iter_;
   }

   bool equal(Triple_iterator const& i) const {
      return fi_ == i.fi_ && si_ == i.si_;
   }

   Triple dereference() const {
      return Converter::get_triple(fi_->first, *si_);
   }
};

/**@brief
*******************************************************************************/
template<
   class Converter,
   class Iter
> class Triple_set_iterator
         : public boost::iterator_facade<
              Triple_set_iterator<Converter, Iter>,
              Triple,
              typename boost::iterator_category<Iter>::type,
              Triple,
              std::ptrdiff_t
           > {
   typedef typename Converter::el0 index;

   Triple_set_iterator(const index ind, const Iter iter)
   : ind_(ind), iter_(iter)
   {}

   index ind_;
   Iter iter_;

   friend class boost::iterator_core_access;
   friend class Triple_find_dispatch;

   void increment() {++iter_;}
   void decrement() {--iter_;}
   void advance(const std::ptrdiff_t n) {iter_ + n;}

   bool equal(Triple_set_iterator const& i) const {
      return ind_ == i.ind_ && iter_ == i.iter_;
   }

   Triple dereference() const {
      return Converter::get_triple(ind_, *iter_);
   }

   std::ptrdiff_t distance_to( Triple_set_iterator const& i) const {
      return i.iter_ - iter_;
   }
};

/**@brief Specialize to search within single set
*******************************************************************************/
template<
   template<class,class> class Map,
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Q1, class Q2, class Q3
> class Triple_find_dispatch<
   Map,
   Tag0,Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag0>::type,Q1,Q2,Q3
> {
   typedef typename boost::mpl::at<Triple, Tag0>::type Q0;
   typedef Convert_fragment<Tag0,Tag1,Tag2,Tag3> convert;
   typedef typename convert::el0 el0;
   typedef typename convert::el1 el1;
   typedef typename convert::el2 el2;
   typedef typename convert::el3 el3;
   typedef Fragment_set<el1, el2, el3> fragment_set;
   typedef Map<el0, fragment_set> storage;
   typedef typename fragment_set::template result<Q1,Q2,Q3> fs_result;
   typedef typename fs_result::iterator fragment_iter;
   typedef typename fs_result::range fragment_range;

public:
   typedef Triple_set_iterator<convert,fragment_iter> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(
            storage const& v,
            Q0 const& q0,
            Q1 const& q1,
            Q2 const& q2,
            Q3 const& q3
   ) {
      fragment_range fr = v[q0].find(q1,q2,q3);
      return range(
               iterator(q0, boost::begin(fr)),
               iterator(q0, boost::end(fr))
      );
   }
};

/**@brief 
*******************************************************************************/
template<
   template<class,class> class Map,
   class Tag0,
   class Tag1,
   class Tag2,
   class Tag3
> class Triple_index {
   typedef boost::mpl::vector4<Tag0,Tag1,Tag2,Tag3> tags;
   BOOST_MPL_ASSERT((
            boost::mpl::equal<typename boost::mpl::sort<tags>::type, triple_tags>
   ));
   typedef Convert_fragment<Tag0,Tag1,Tag2,Tag3> converter;
   typedef typename converter::el0 el0;
   typedef typename converter::el1 el1;
   typedef typename converter::el2 el2;
   typedef typename converter::el3 el3;
   typedef typename converter::fragment fragment;
   typedef Fragment_set<el1, el2, el3> fragment_set;
   typedef Map<el0, fragment_set> storage;

public:

   template<class Q0, class Q1, class Q2, class Q3> struct result {
      typedef typename
               Triple_find_dispatch<Map,Tag0,Tag1,Tag2,Tag3,Q0,Q1,Q2,Q3>::range
               range;
   };

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
      v_.insert(converter::get_index(t), converter::get_fragment(t));
   }

   std::size_t size() const {return v_.n_fragments();}

   bool empty() const {return ! v_.n_fragments();}

   template<class Q0, class Q1, class Q2, class Q3>
   typename result<Q0,Q1,Q2,Q3>::range
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
      return Triple_find_dispatch<Map,Tag0,Tag1,Tag2,Tag3,qt0,qt1,qt2,qt3>::find(
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
