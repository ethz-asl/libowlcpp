/** @file "/owlcpp/include/owlcpp/detail/iterator_member_pair.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ITERATOR_MEMBER_PAIR_HPP_
#define ITERATOR_MEMBER_PAIR_HPP_
#include "boost/iterator/iterator_facade.hpp"

namespace owlcpp{

/**@brief Iterate over container of std::pair-s, but point to pair members

This is a hack to compensate for non-standard MSVC std::pair.
Normally, Member_iterator should have been used.
@code
error C2440: 'specialization' : cannot convert from
'Ns_id std::_Pair_base<_Ty1,_Ty2>::* ' to 'Ns_id std::pair<_Ty1,_Ty2>::* '
@endcode

@tparam Iter base iterator over pairs
@tparam Value member type
@tparam N 1 or 2

Example:
@code
typedef Iterator_member_pair<std::map<int,double>::iterator, const int, &std::pair<int,double>::first> int_iterator_t;
@endcode
*******************************************************************************/
template<class Iter, class Value, unsigned N>class Iterator_member_pair{};

template<class Iter, class Value>
class Iterator_member_pair<Iter, Value, 1>
: public boost::iterator_facade<
     Iterator_member_pair<Iter,Value,1>,
     Value,
     boost::forward_traversal_tag
   > {

public:
   typedef Iter base_iterator;

   Iterator_member_pair(base_iterator iter) : iter_(iter) {}

private:
   base_iterator iter_;

   friend class boost::iterator_core_access;
   void increment() { ++iter_; }

   bool equal(Iterator_member_pair const& i) const {return iter_ == i.iter_;}

   Value& dereference() const { return iter_->first; }
};

template<class Iter, class Value>
class Iterator_member_pair<Iter, Value, 2>
: public boost::iterator_facade<
     Iterator_member_pair<Iter,Value,2>,
     Value,
     boost::forward_traversal_tag
   > {

public:
   typedef Iter base_iterator;

   Iterator_member_pair(base_iterator iter) : iter_(iter) {}

private:
   base_iterator iter_;

   friend class boost::iterator_core_access;
   void increment() { ++iter_; }

   bool equal(Iterator_member_pair const& i) const {return iter_ == i.iter_;}

   Value& dereference() const { return iter_->second; }
};

}//namespace owlcpp
#endif /* ITERATOR_MEMBER_PAIR_HPP_ */
