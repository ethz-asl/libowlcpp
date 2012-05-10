/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_INDEX_HPP_
#define TRIPLE_INDEX_HPP_
#include <vector>
#include "boost/array.hpp"
#include "boost/assert.hpp"
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/range.hpp"
#include "boost/mpl/vector_c.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/back.hpp"
#include "boost/mpl/plus.hpp"
#include "boost/mpl/int.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/pop_back.hpp"

#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace query_detail{
namespace mpl = boost::mpl;


/**@brief 
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> struct Index_config{
   typedef mpl::vector_c<bool, Subj, Pred, Obj, Doc> is_indexed;

   typedef typename mpl::fold<
            typename mpl::pop_back<is_indexed>::type,
            mpl::vector_c<int,0>,
            mpl::if_<
               mpl::_2,
               mpl::push_back<mpl::_1, mpl::plus<mpl::back<mpl::_1>, mpl::int_<1> > >,
               mpl::push_back<mpl::_1, mpl::back<mpl::_1> >
            >
   >::type position;

   typedef typename mpl::fold<
            is_indexed,
            mpl::int_<0>,
            mpl::if_<mpl::_2, mpl::next<mpl::_1>, mpl::_1>
   >::type n_indexed;
};

/**@brief
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> class Triple_index {
public:
   typedef Index_config<Subj, Pred, Obj, Doc> config_t;
private:
   typedef std::vector<Triple const*> index_t;
   typedef boost::array<index_t, config_t::n_indexed::value> value_t;
   typedef std::vector<value_t> indices_t;
   typedef typename config_t::position pos_t;
public:
   typedef boost::indirect_iterator<index_t::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;

   void insert(Triple const& t) {
      resize(t);
      if( Subj ) ind_[t.subject()()  ][mpl::at_c<pos_t, 0>::type::value].push_back(&t);
//      if( Pred )ind_[t.predicate()()][mpl::at_c<pos_t, 1>::type::value].push_back(&t);
//      if( Obj ) ind_[t.object()()   ][mpl::at_c<pos_t, 2>::type::value].push_back(&t);
//      if( Doc ) ind_[t.document()() ][mpl::at_c<pos_t, 3>::type::value].push_back(&t);
   }

   template<std::size_t N> range find(const std::size_t n) const {
      if( ! mpl::at_c<typename config_t::is_indexed, N>::type::value ) return range();
      return ind_[n][mpl::at_c<typename config_t::position, N>::type::value];
   }

private:
   indices_t ind_;

   void resize(Triple const& t) {
      std::size_t s = ind_.size();
      if( Subj && t.subject()()   >= s ) s = t.subject()() + 1;
      if( Pred && t.predicate()() >= s ) s = t.predicate()() + 1;
      if( Obj  && t.object()()    >= s ) s = t.object()() + 1;
      if( Doc  && t.document()()  >= s ) s = t.document()() + 1;
      if( s > ind_.size() ) ind_.resize(s);
   }
};

}//namespace query_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
