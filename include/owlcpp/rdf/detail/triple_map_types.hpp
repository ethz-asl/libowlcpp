/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_map_types.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_TYPES_HPP_
#define TRIPLE_MAP_TYPES_HPP_
#include <vector>
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/tuple/tuple.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace triple_map{

/**@brief 
*******************************************************************************/
class Index_val {
public:
   typedef std::vector<Triple const*> index_t;
   typedef boost::indirect_iterator<index_t::iterator, Triple> iterator;
//   typedef boost::indirect_iterator<index_t::const_iterator, Triple> const_iterator;


private:
   index_t index_;
};

/**@brief
*******************************************************************************/
class Index {
   typedef std::vector<Triple const*> index_t;
   typedef boost::tuple<index_t,index_t,index_t> value_t;
   typedef std::vector<value_t> stor_t;
public:

   void insert(Triple const* const t) {
      const Node_id subj = t->subject();
      const Node_id pred = t->predicate();
      const Node_id obj = t->object();
      const Node_id max = subj > pred ? std::max(subj, obj) : std::max(pred, obj) ;
      if( max() >= stor_.size() ) stor_.resize(max() + 1);
      stor_[subj()].get<0>().push_back(t);
      stor_[pred()].get<1>().push_back(t);
      stor_[obj() ].get<2>().push_back(t);
   }

private:
   stor_t stor_;
};

}//namespace triple_map
}//namespace owlcpp
#endif /* TRIPLE_MAP_TYPES_HPP_ */
