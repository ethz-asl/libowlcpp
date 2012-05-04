/** @file "/owlcpp/include/owlcpp/detail/transform_iterator.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRANSFORM_ITERATOR_HPP_
#define TRANSFORM_ITERATOR_HPP_
#include "boost/iterator/iterator_adaptor.hpp"

namespace owlcpp{

/**@brief Iterator returning result of a unary function class
@tparam Iter base iterator over structures
@tparam Value member type
@tparam Member pointer to structure member
*******************************************************************************/
template<class Fun, class Iter>
class Transform_iterator
: public boost::iterator_adaptor<
  Transform_iterator<Fun, Iter>,
  Iter,
  boost::use_default,
  boost::use_default,
  typename Fun::result_type
  > {

   typedef boost::iterator_adaptor<
            Transform_iterator<Fun,Iter>,
            Iter,
            boost::use_default,
            boost::use_default,
            typename Fun::result_type
            >
   super_t;
   friend class boost::iterator_core_access;
public:
   Transform_iterator() {}
   Transform_iterator(Iter i) : super_t(i) {}

private:
   typename Fun::result_type dereference() const {
      return Fun()(this->base());
   }
};


}//namespace owlcpp
#endif /* TRANSFORM_ITERATOR_HPP_ */
