/** @file "/owlcpp/include/owlcpp/rdf/detail/fragment_map_vector.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef FRAGMENT_MAP_VECTOR_HPP_
#define FRAGMENT_MAP_VECTOR_HPP_
#include <vector>
#include <utility>
#include "boost/assert.hpp"
#include "boost/iterator/iterator_facade.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief 
*******************************************************************************/
template<class Id, class Set> class Fragment_map_vector {
   typedef std::vector<Set> storage;

   static Set const& empty_set() {
      static const Set s;
      return s;
   }

public:
   typedef typename Set::value_type fragment;
   typedef std::pair<Id, Set const&> value_type;

   class iterator : public boost::iterator_facade<
      iterator,
      value_type,
      boost::random_access_traversal_tag,
      value_type
   > {
      typedef typename storage::const_iterator base_iter;

      explicit iterator(const base_iter begin) : begin_(begin), i_(begin) {}
      iterator(const base_iter begin, const base_iter i)
      : begin_(begin), i_(i) {}

      base_iter begin_;
      base_iter i_;

      friend class boost::iterator_core_access;
      friend class Fragment_map_vector;

      void increment() {++i_;}

      bool equal(iterator const& i) const {
         return begin_ == i.begin_ && i_ == i.i_;
      }

      value_type dereference() const {
         BOOST_ASSERT(begin_ <= i_);
         const Id id(static_cast<unsigned>(i_ - begin_));
         return value_type(id, *i_);
      }
   };

   typedef iterator const_iterator;

   Fragment_map_vector() : s_(), count_(0) {}

   const_iterator begin() const {return const_iterator(s_.begin());}
   const_iterator end() const {return const_iterator(s_.begin(), s_.end());}

   std::size_t n_fragments() const {return count_;}

   void insert(const Id id, fragment const& v) {
      if( id() >= s_.size() ) s_.resize(id() + 1);
      if( s_[id()].insert(v) ) ++count_;
   }

   Set const& operator[](const Id id) const {
      if( id() >= s_.size() ) return empty_set();
      return s_[id()];
   }

private:
   storage s_;
   std::size_t count_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* FRAGMENT_MAP_VECTOR_HPP_ */
