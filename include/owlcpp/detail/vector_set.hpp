/** @file "/owlcpp/include/owlcpp/detail/vector_set.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef VECTOR_SET_HPP_
#define VECTOR_SET_HPP_
#include <vector>

namespace owlcpp{
/**@brief Collection of unique objects stored in a vector
*******************************************************************************/
template<class T, class Comp = std::less<T>, class Alloc = std::allocator<T> >
class Vector_set {
   typedef std::vector<T, Alloc> stor_t;
public:
   typedef T value_type;
   typedef typename stor_t::iterator iterator;
   typedef typename stor_t::const_iterator const_iterator;

   Vector_set() : v_(), comp_() {}
   Vector_set(Comp const& comp) : v_(), comp_(comp) {}
   iterator begin() {return v_.begin();}
   iterator end() {return v_.end();}
   const_iterator begin() const {return v_.begin();}
   const_iterator end() const {return v_.end();}
   std::size_t size() const {return v_.size();}

   std::pair<iterator, bool> insert(value_type const& t) {
      iterator i = std::lower_bound(v_.begin(), v_.end(), t, comp_);
      if( i == v_.end() || comp_(t, *i) ) {
         return std::make_pair(v_.insert(i, t), true);
      }
      return std::make_pair(i, false);
   }

   const_iterator find(value_type const& t) const {
      const_iterator i = std::lower_bound(v_.begin(), v_.end(), t, comp_);
      if( i == v_.end() || comp_(t, *i) ) return v_.end();
      return i;
   }

   value_type const& operator[](const std::size_t n) const {return v_[n];}
   value_type const& at(const std::size_t n) const {return v_.at(n);}

private:
   stor_t v_;
   Comp comp_;
};

}//namespace owlcpp
#endif /* VECTOR_SET_HPP_ */
