/** @file "/owlcpp/include/owlcpp/detail/member_iterator.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MEMBER_ITERATOR_HPP_
#define MEMBER_ITERATOR_HPP_

#include "boost/iterator/iterator_adaptor.hpp"

namespace owlcpp{

/**@brief Iterate over container of structures, but point to structure members
@tparam Iter base iterator over structures
@tparam Value member type
@tparam Member pointer to structure member
@code
typedef Member_iterator<std::map<int,double>::iterator, const int, &std::pair<int,double>::first> int_iterator_t;
@endcode
*******************************************************************************/
template<class Iter, class Value, Value Iter::value_type::*Member>
class Member_iterator
: public boost::iterator_adaptor<Member_iterator<Iter, Value, Member>, Iter, Value> {

   typedef boost::iterator_adaptor<Member_iterator<Iter, Value, Member>, Iter, Value> super_t;
   friend class boost::iterator_core_access;
public:
   Member_iterator() : super_t(Iter()) {}
   Member_iterator(Iter i) : super_t(i) {}

private:
   typename super_t::reference dereference() const {
      return (*this->base()).*Member;
   }
};


}//namespace owlcpp
#endif /* MEMBER_ITERATOR_HPP_ */
