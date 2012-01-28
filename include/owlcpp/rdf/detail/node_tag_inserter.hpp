/** @file "/owlcpp/include/owlcpp/rdf/detail/node_tag_inserter.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TAG_INSERTER_HPP_
#define NODE_TAG_INSERTER_HPP_

#include "owlcpp/rdf/node_map.hpp"

namespace owlcpp{ namespace detail{

/**@brief Insert standard node tags into node store
*******************************************************************************/
class Node_tag_inserter {
   Node_tag_inserter();
public:
   explicit Node_tag_inserter(Node_map& store) : store_(&store) {}

   template<class T> void operator()(T const&) const {
      store_->insert( T::id(), Node(T::ns_type::id(), T::name()) );
   }

private:
   mutable Node_map* store_;
};

}//namespace detail
}//namespace owlcpp
#endif /* NODE_TAG_INSERTER_HPP_ */
