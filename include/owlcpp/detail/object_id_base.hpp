/** @file "/owlcpp/include/owlcpp/detail/object_id_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OBJECT_ID_BASE_HPP_
#define OBJECT_ID_BASE_HPP_
#include <iosfwd>
#include "boost/cstdint.hpp"

namespace owlcpp{ namespace detail{

/**
*******************************************************************************/
template<class Super, class Val = boost::uint_least32_t> class Base_id {
protected:
   typedef Base_id base;
public:
   typedef Val value_type;
   explicit Base_id(const value_type val) : val_(val) {}
   Base_id(Super const& s) : val_(s.val_) {}
   bool operator==(const Super& t) const {return val_ == t.val_;}
   bool operator<(const Super& t) const {return val_< t.val_;}
   bool operator<=(const Super& t) const {return val_<= t.val_;}
   bool operator>(const Super& t) const {return val_ > t.val_;}
   bool operator>=(const Super& t) const {return val_ >= t.val_;}
   bool operator!=(const Super& t) const {return val_ != t.val_;}
   value_type operator()() const {return val_;}

private:
   value_type val_;
};

/**
*******************************************************************************/
template<class ChT, class Tr, class S, class V> inline
std::basic_ostream<ChT,Tr>& operator<<(
      std::basic_ostream<ChT,Tr>& os,
      Base_id<S,V> const& id
) {
   return os << id();
}

/**
*******************************************************************************/
template<class S,class V> inline std::size_t hash_value(Base_id<S,V> const& id) {
   return id();
}

}//namespace detail
}//namespace owlcpp
#endif /* OBJECT_ID_BASE_HPP_ */
