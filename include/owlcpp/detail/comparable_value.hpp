/** @file "/owlcpp/include/owlcpp/detail/comparable_value.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef COMPARABLE_VALUE_HPP_
#define COMPARABLE_VALUE_HPP_
#include <iosfwd>

namespace owlcpp{ namespace detail{

/** Use as a base for making type-specific IDs for large objects
*******************************************************************************/
template<class T> class Comparable_value {
public:
   typedef T value_type;
   bool operator==(const Comparable_value& t) const {return val_ == t.val_;}
   bool operator<(const Comparable_value& t) const {return val_< t.val_;}
   bool operator<=(const Comparable_value& t) const {return val_<= t.val_;}
   bool operator>(const Comparable_value& t) const {return val_ > t.val_;}
   bool operator>=(const Comparable_value& t) const {return val_ >= t.val_;}
   bool operator!=(const Comparable_value& t) const {return val_ != t.val_;}
   T operator()() const {return val_;}

protected:
   Comparable_value() : val_() {}
   explicit Comparable_value(const T val) : val_(val) {}
   Comparable_value(const Comparable_value& cv) : val_(cv.val_) {}
   T& value() {return val_;}
private:
   T val_;
};

/**
*******************************************************************************/
template<class ChT, class Tr, class T> inline std::basic_ostream<ChT,Tr>& operator<<(
      std::basic_ostream<ChT,Tr>& os, Comparable_value<T> const& cv) {
   return os << cv();
}

/**
*******************************************************************************/
template<class T> inline std::size_t hash_value(Comparable_value<T> const& cv) {
   return cv();
}

typedef Comparable_value<unsigned> unsigned_val_t;

}//namespace detail
}//namespace owlcpp
#endif /* COMPARABLE_VALUE_HPP_ */
