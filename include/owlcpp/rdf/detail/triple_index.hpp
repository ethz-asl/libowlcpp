/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_HPP_
#define TRIPLE_INDEX_HPP_
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/iterator/iterator_facade.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/mpl/sort.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/vector_c.hpp"
#include "boost/type_traits/has_equal_to.hpp"
#include "boost/type_traits/remove_reference.hpp"

#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/detail/adapt_triple.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/print_triple.hpp"
#include "owlcpp/rdf/triple_tags.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief
*******************************************************************************/
template<
   class Id_set_iter,
   class Q1, class Q2, class Q3
> class Triple_merge_iterator
         : public boost::iterator_facade<
              Triple_merge_iterator<Id_set_iter, Q1, Q2, Q3>,
              Triple,
              boost::forward_traversal_tag,
              Triple const&
           > {
   typedef typename Id_set_iter::value_type value1;
   typedef typename boost::remove_reference<typename value1::second_type>::type
            triple_set;
   typedef typename triple_set::template query<Q1,Q2,Q3> query;
   typedef typename query::range t_range;

public:
   Triple_merge_iterator(const Id_set_iter begin, const Id_set_iter end,
            Q1 const& q1, Q2 const& q2, Q3 const& q3)
   : begin_(begin),
     end_(end),
     q1_(q1),
     q2_(q2),
     q3_(q3),
     tr_(get_fragment_range())
     {
      ensure_end_or_match();
     }

private:
   Id_set_iter begin_;
   Id_set_iter end_;
   Q1 q1_;
   Q2 q2_;
   Q3 q3_;
   t_range tr_;

   friend class boost::iterator_core_access;

   void increment() {
      tr_.advance_begin(1);
      ensure_end_or_match();
   }

   bool equal(Triple_merge_iterator const& i) const {
      return begin_ == i.begin_ && tr_.begin() == i.tr_.begin();
   }

   Triple const& dereference() const {
      return tr_.front();
   }

   void ensure_end_or_match() {
      while( ! tr_ && (begin_ != end_) ) {
         ++begin_;
         tr_ = get_fragment_range();
      }
   }

   t_range get_fragment_range() {
      if( begin_ == end_ ) return triple_set::empty_set().find(q1_,q2_,q3_);
      return begin_->second.find(q1_,q2_,q3_);
   }
};

/**@brief relative diversity of elements in each triple position
*******************************************************************************/
struct Element_diversity : public boost::mpl::vector4_c<int,4, 2, 3, 1> {};

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
   template<class,class,class,class> class Map,
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Q0, class Q1, class Q2, class Q3
> class Query_dispatch {
   typedef Map<Tag0,Tag1,Tag2,Tag3> storage;
   typedef typename storage::template query<Q0> query1;
   typedef typename query1::iterator fs_iter;
   typedef typename query1::range fs_range;
   typedef typename storage::set_type::template query<Q1, Q2, Q3> query2;

public:
   typedef Triple_merge_iterator<fs_iter, Q1, Q2, Q3> iterator;
   typedef boost::iterator_range<iterator> range;
   static const int efficiency =
            query2::efficiency -
            boost::mpl::at<Element_diversity, Tag0>::type::value;

   static range find(
            storage const& v,
            Q0 const& q0,
            Q1 const& q1,
            Q2 const& q2,
            Q3 const& q3
   ) {
      const fs_range r = v.find(q0);
      return range(
               iterator(r.begin(), r.end(), q1,q2,q3),
               iterator(r.end(), r.end(), q1,q2,q3)
      );
   }
};

/**@brief Specialize to search within single set
*******************************************************************************/
template<
   template<class,class,class,class> class Map,
   class Tag0, class Tag1, class Tag2, class Tag3,
   class Q1, class Q2, class Q3
> class Query_dispatch<
   Map,
   Tag0,Tag1,Tag2,Tag3,
   typename boost::mpl::at<Triple, Tag0>::type,
   Q1,Q2,Q3
> {
   typedef typename boost::mpl::at<Triple, Tag0>::type Q0;
   typedef Map<Tag0,Tag1,Tag2,Tag3> storage;
   typedef typename storage::set_type set_type;
   typedef typename set_type::template query<Q1,Q2,Q3> query;

public:
   typedef typename query::iterator iterator;
   typedef typename query::range range;
   static const int efficiency =
            query::efficiency +
            100 +
            boost::mpl::at<Element_diversity, Tag0>::type::value;

   static range find(
            storage const& v,
            Q0 const& q0,
            Q1 const& q1,
            Q2 const& q2,
            Q3 const& q3
   ) {
      return v[q0].find(q1,q2,q3);
   }
};

/**@brief Store RDF triples in a searchable fashion
@tparam Map a map between Tag0 elements and <Tag1,Tag2,Tag3> triple fragments
@tparam Tag0 numerical tag indicating which triple element is indexed first
@tparam Tag1 numerical tag indicating which triple element is indexed second
@tparam Tag2 numerical tag indicating which triple element is indexed third
@tparam Tag3 numerical tag indicating which triple element is indexed fourth
*******************************************************************************/
template<
   template<class,class,class,class> class Map,
   class Tag0,
   class Tag1,
   class Tag2,
   class Tag3
> class Triple_index {
   typedef boost::mpl::vector4<Tag0,Tag1,Tag2,Tag3> sort_order;
   BOOST_MPL_ASSERT((
            boost::mpl::equal<
               typename boost::mpl::sort<sort_order>::type,
               boost::mpl::vector4<Subj_tag, Pred_tag, Obj_tag, Doc_tag>
            >
   ));
   typedef typename boost::mpl::at<Triple,Tag0>::type el0;
   typedef typename boost::mpl::at<Triple,Tag1>::type el1;
   typedef typename boost::mpl::at<Triple,Tag2>::type el2;
   typedef typename boost::mpl::at<Triple,Tag3>::type el3;
   typedef Map<Tag0,Tag1,Tag2,Tag3> storage;

public:

   template<class Subj, class Pred, class Obj, class Doc> class query {
      typedef boost::fusion::vector4<Subj,Pred,Obj,Doc> bfvect_t;
      typedef typename boost::mpl::at<bfvect_t, Tag0>::type qt0;
      typedef typename boost::mpl::at<bfvect_t, Tag1>::type qt1;
      typedef typename boost::mpl::at<bfvect_t, Tag2>::type qt2;
      typedef typename boost::mpl::at<bfvect_t, Tag3>::type qt3;
   public:
      typedef Query_dispatch<
                  Map,Tag0,Tag1,Tag2,Tag3, qt0, qt1, qt2, qt3
               > dispatch;
      typedef typename dispatch::range range;
      typedef typename dispatch::iterator iterator;
      static const int efficiency = dispatch::efficiency;

      static range find(
               storage const& v,
               Subj const& subj, Pred const& pred,
               Obj const& obj, Doc const& doc
      ) {
         BOOST_MPL_ASSERT((boost::has_equal_to<el0,qt0,bool>));
         BOOST_MPL_ASSERT((boost::has_equal_to<el1,qt1,bool>));
         BOOST_MPL_ASSERT((boost::has_equal_to<el2,qt2,bool>));
         BOOST_MPL_ASSERT((boost::has_equal_to<el3,qt3,bool>));
         const bfvect_t q(subj, pred, obj, doc);
         return dispatch::find(
                  v,
                  boost::fusion::at<Tag0>(q),
                  boost::fusion::at<Tag1>(q),
                  boost::fusion::at<Tag2>(q),
                  boost::fusion::at<Tag3>(q)
         );
      }
   };

   typedef typename query<Any,Any,Any,Any>::iterator iterator;
   typedef iterator const_iterator;
   std::size_t size() const {return v_.n_fragments();}
   bool empty() const {return ! v_.n_fragments();}

   const_iterator begin() const {
      return const_iterator(v_.begin(), v_.end(), any,any,any);
   }

   const_iterator end() const {
      return const_iterator(v_.end(), v_.end(), any,any,any);
   }

   template<class Subj, class Pred, class Obj, class Doc>
   typename query<Subj,Pred,Obj,Doc>::range
   find(Subj const& subj, Pred const& pred, Obj const& obj, Doc const& doc) const {
      return query<Subj,Pred,Obj,Doc>::find(v_, subj, pred, obj, doc);
   }

   bool insert(Triple const& t) {return v_.insert(t);}

   void erase(Triple const& t) {
      try{v_.erase(t);} catch(Rdf_err const&) {
         BOOST_THROW_EXCEPTION(
                  Rdf_err()
                  << Rdf_err::msg_t("triple not found")
                  << Rdf_err::str1_t(to_string(t))
         );
      }
   }

   void clear() {v_.clear();}

private:
   storage v_;
};

/**@brief Insert triple into index
*******************************************************************************/
class Insert {
public:
   Insert(Triple const& t, bool& inserted) : t_(t), inserted_(inserted) {}

   template<class Index> void operator()(Index& i) const {
      inserted_ = i.insert(t_);
   }

private:
   Triple const& t_;
   bool& inserted_;
};

/**@brief Erase triple from index
*******************************************************************************/
class Erase {
public:
   Erase(Triple const& t) : t_(t) {}
   template<class Index> void operator()(Index& i) const {i.erase(t_);}
private:
   Triple const& t_;
};

/**@brief Clear index
*******************************************************************************/
struct Clear {
   template<class Index> void operator()(Index& i) const {i.clear();}
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
