/** @file "/owlcpp/include/owlcpp/rdf/detail/fragment_map_vector.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef FRAGMENT_MAP_VECTOR_HPP_
#define FRAGMENT_MAP_VECTOR_HPP_
#include <vector>
#include <utility>
#include <functional>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/iterator/iterator_facade.hpp"
#include "boost/range.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/any_triple_element.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief 
*******************************************************************************/
template<class Id, class Iter> class Fvm_iterator
         : public boost::iterator_facade<
              Fvm_iterator<Id,Iter>,
              std::pair<Id, typename Iter::value_type const&>,
              boost::random_access_traversal_tag,
              std::pair<Id, typename Iter::value_type const&>
> {
public:
   typedef std::pair<Id, typename Iter::value_type const&> value_type;

   Fvm_iterator(const Iter begin, const Iter i)
   : begin_(begin), i_(i) {}

private:
   Iter begin_;
   Iter i_;

   friend class boost::iterator_core_access;

   void increment() {++i_;}

   bool equal(Fvm_iterator const& i) const {
      BOOST_ASSERT(begin_ == i.begin_ && "only compare iterators for same container");
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
template<class Id, class Set> struct Fmv_config {
   typedef typename Set::value_type fragment;
   typedef std::pair<Id, Set const&> value_type;
   typedef std::vector<Set> storage;
   typedef Fvm_iterator<Id, typename storage::const_iterator> iterator;
};

/**@brief
*******************************************************************************/
template<class Id, class Set, class Q> class Fmv_search_dispatch {
   typedef Fmv_config<Id,Set> config;
   typedef typename config::storage storage;
   typedef typename config::value_type value_type;

   class Equal : public std::unary_function<value_type, bool> {
   public:
      explicit Equal(const Q q) : q_(q) {}
      bool operator()(value_type const& p) const {return p.first == q_;}
   private:
      Q q_;
   };

public:
   typedef boost::filter_iterator<Equal, typename config::iterator> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, const Q q) {
      const Equal eq(q);
      return range(
               iterator(eq, s.begin(), s.end()),
               iterator(eq, s.end(), s.end())
      );
   }
};

template<class Id, class Set> class Fmv_search_dispatch<Id,Set,Id> {
   typedef Fmv_config<Id,Set> config;
   typedef typename config::storage storage;
public:
   typedef typename config::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, const Id id) {
      if( id() >= s.size() ) {
         return range(iterator(s.end()), iterator(s.end()));
      }
      return range(
               iterator(s.begin() + id()),
               iterator(s.begin() + id() + 1)
      );
   }
};

template<class Id, class Set> class Fmv_search_dispatch<Id,Set,any> {
   typedef Fmv_config<Id,Set> config;
   typedef typename config::storage storage;
public:
   typedef typename config::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, any const&) {
      return range(
               iterator(s.begin(), s.begin()),
               iterator(s.begin(), s.end())
      );
   }
};

/**@brief Container of @b Set -s mapped against @b ID -s
@details
*******************************************************************************/
template<class Id, class Set> class Fragment_map_vector {
   typedef Fmv_config<Id,Set> config;
   typedef typename config::storage storage;

public:
   typedef typename config::fragment fragment;
   typedef typename config::value_type value_type;
   typedef typename config::iterator iterator;
   typedef iterator const_iterator;
   typedef boost::iterator_range<iterator> range;

   const_iterator begin() const {return const_iterator(s_.begin(), s_.begin());}
   const_iterator end() const {return const_iterator(s_.begin(), s_.end());}

   std::size_t n_fragments() const {
      std::size_t n = 0;
      BOOST_FOREACH(Set const& set, s_) {
         n += set.size();
      }
      return n;
   }

   bool insert(const Id id, fragment const& f) {
      if( id() >= s_.size() ) s_.resize(id() + 1);
      return s_[id()].insert(f);
   }

   void erase(const Id id, fragment const& f) {
      if( id() >= s_.size() ) BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("element not found")
               << Rdf_err::int1_t(id())
      );
      s_[id()].erase(f);
   }

   void clear() {s_.clear();}

   Set const& operator[](const Id id) const {
      if( id() >= s_.size() ) return Set::empty_set();
      return s_[id()];
   }

   template<class Q> struct query {
      typedef typename Fmv_search_dispatch<Id,Set,Q>::iterator iterator;
      typedef typename Fmv_search_dispatch<Id,Set,Q>::range range;
   };

   template<class Q> typename query<Q>::range
   find(Q const& q) const {
      return Fmv_search_dispatch<Id,Set,Q>::find(s_, q);
   }

private:
   storage s_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* FRAGMENT_MAP_VECTOR_HPP_ */
