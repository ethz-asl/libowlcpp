/** @file "/owlcpp/include/owlcpp/detail/member_iterator.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MEMBER_ITERATOR_HPP_
#define MEMBER_ITERATOR_HPP_

#include "boost/iterator/iterator_facade.hpp"

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
: public boost::iterator_facade<
     Member_iterator<Iter,Value,Member>,
     Value,
     boost::forward_traversal_tag
   > {

public:
   typedef Iter base_iterator;

   Member_iterator(base_iterator iter) : iter_(iter) {}

private:
   base_iterator iter_;

   friend class boost::iterator_core_access;
   void increment() { ++iter_; }

   bool equal(Member_iterator const& i) const {return iter_ == i.iter_;}

   Value& dereference() const { return (*iter_).*Member; }
};

}//namespace owlcpp
#endif /* MEMBER_ITERATOR_HPP_ */
