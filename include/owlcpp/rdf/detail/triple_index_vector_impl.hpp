/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_vector_impl.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_VECTOR_IMPL_HPP_
#define TRIPLE_INDEX_VECTOR_IMPL_HPP_
#include <functional>
#include <utility>
#include <vector>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/iterator/iterator_facade.hpp"
#include "boost/mpl/at.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/detail/triple_set.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Iterator for a vector of triple indices
*******************************************************************************/
template<class Id, class Iter> class Tiv_iterator
         : public boost::iterator_facade<
              Tiv_iterator<Id,Iter>,
              std::pair<Id, typename Iter::value_type const&>,
              boost::random_access_traversal_tag,
              std::pair<Id, typename Iter::value_type const&>
> {
public:
   typedef std::pair<Id, typename Iter::value_type const&> value_type;

   Tiv_iterator(const Iter begin, const Iter i)
   : begin_(begin), i_(i) {}

private:
   Iter begin_;
   Iter i_;

   friend class boost::iterator_core_access;

   void increment() {++i_;}

   bool equal(Tiv_iterator const& i) const {
      BOOST_ASSERT(
               begin_ == i.begin_ &&
               "only compare iterators for same container"
      );
      return  i_ == i.i_;
   }

   value_type dereference() const {
      BOOST_ASSERT(begin_ <= i_);
      const Id id(static_cast<unsigned>(i_ - begin_));
      return value_type(id, *i_);
   }
};

/**@brief Fragment map vector configuration
*******************************************************************************/
template<class Tag0,class Tag1, class Tag2, class Tag3> struct Tiv_config {
   typedef typename boost::mpl::at<Triple, Tag0>::type id_type;
   typedef Triple_set<Tag1,Tag2,Tag3> set_type;
   typedef std::pair<id_type, set_type const&> value_type;
   typedef std::vector<set_type> storage;
   typedef Tiv_iterator<id_type, typename storage::const_iterator> iterator;
};

/**@brief
*******************************************************************************/
template<class Tag0,class Tag1, class Tag2, class Tag3, class Q0>
class Tiv_query_dispatch {
   typedef Tiv_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
   typedef typename config::set_type set_type;
   typedef typename config::id_type id_type;
   typedef typename config::value_type value_type;

   class Equal : public std::unary_function<value_type, bool> {
   public:
      explicit Equal(const Q0 q) : q_(q) {}
      bool operator()(value_type const& p) const {return p.first == q_;}
   private:
      Q0 q_;
   };

public:
   typedef boost::filter_iterator<Equal, typename config::iterator> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, const Q0 q0) {
      const Equal eq(q0);
      return range(
               iterator(eq, s.begin(), s.end()),
               iterator(eq, s.end(), s.end())
      );
   }
};

template<class Tag0,class Tag1, class Tag2, class Tag3>
class Tiv_query_dispatch<Tag0,Tag1,Tag2,Tag3,Any> {
   typedef Tiv_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
public:
   typedef typename config::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, Any const&) {
      return range(
               iterator(s.begin(), s.begin()),
               iterator(s.begin(), s.end())
      );
   }
};

/**@brief Container of @b Set -s mapped against @b ID -s
@details
*******************************************************************************/
template<class Tag0,class Tag1, class Tag2, class Tag3>
class Triple_index_vector_impl {
   typedef Tiv_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;

public:
   typedef typename config::set_type set_type;
   typedef typename config::id_type id_type;
   typedef typename config::value_type value_type;
   typedef typename config::iterator iterator;
   typedef iterator const_iterator;
   typedef boost::iterator_range<iterator> range;

   const_iterator begin() const {return const_iterator(s_.begin(), s_.begin());}
   const_iterator end() const {return const_iterator(s_.begin(), s_.end());}

   std::size_t n_fragments() const {
      std::size_t n = 0;
      BOOST_FOREACH(set_type const& set, s_) {n += set.size();}
      return n;
   }

   bool insert(Triple const& t) {
      const id_type id = boost::fusion::at<Tag0>(t);
      if( id() >= s_.size() ) s_.resize(id() + 1);
      return s_[id()].insert(t);
   }

   void erase(Triple const& t) {
      const id_type id = boost::fusion::at<Tag0>(t);
      if( id() >= s_.size() ) BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("element not found")
               << Rdf_err::int1_t(id())
      );
      s_[id()].erase(t);
   }

   void clear() {s_.clear();}

   set_type const& operator[](const id_type id) const {
      if( id() >= s_.size() ) return set_type::empty_set();
      return s_[id()];
   }

   template<class Q0> struct query {
      typedef typename Tiv_query_dispatch<Tag0,Tag1,Tag2,Tag3,Q0>::iterator
               iterator;

      typedef typename Tiv_query_dispatch<Tag0,Tag1,Tag2,Tag3,Q0>::range
               range;
   };

   template<class Q0> typename query<Q0>::range
   find(Q0 const& q) const {
      return Tiv_query_dispatch<Tag0,Tag1,Tag2,Tag3,Q0>::find(s_, q);
   }

private:
   storage s_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_VECTOR_IMPL_HPP_ */
