/** @file "/owlcpp/include/owlcpp/detail/id_tracker.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#ifndef ID_TRACKER_HPP_
#define ID_TRACKER_HPP_
#include <stack>
#include "boost/assert.hpp"

namespace owlcpp{ namespace detail{

/** Generate new IDs and keep track of IDs no longer needed
*******************************************************************************/
template<class Id> class Id_tracker {
public:
   typedef Id id_type;
private:
   typedef typename id_type::value_type counter_type;

public:
   Id_tracker() : counter_(0) {}

   Id_tracker(const id_type id) : counter_(id() + 1) {}

   id_type get() {
      if( stack_.empty() ) return id_type(counter_++);
      const id_type id = stack_.top();
      stack_.pop();
      return id;
   }

   void push(id_type const id) {
      BOOST_ASSERT(id < id_type(counter_));
      stack_.push(id);
   }

private:
   counter_type counter_;
   std::stack<id_type> stack_;
};

}//namespace detail
}//namespace owlcpp
#endif /* ID_TRACKER_HPP_ */
