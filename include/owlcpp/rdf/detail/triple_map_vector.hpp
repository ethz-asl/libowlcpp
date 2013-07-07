/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_map_vector.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_MAP_VECTOR_HPP_
#define TRIPLE_MAP_VECTOR_HPP_
#include <vector>
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/iterator/iterator_facade.hpp"
#include "owlcpp/rdf/detail/triple_index_config.hpp"

namespace owlcpp{ namespace map_triple_detail{

template<class Id, class Set> class Tiv_iterator
         : public boost::iterator_facade<
              Tiv_iterator<Id,Set>,
              std::pair<Id, Set const&>,
              boost::use_default,
              std::pair<Id, Set const&>
           > {

};

/**@brief 
*******************************************************************************/
template<
   class Tag0,
   class Tag1,
   class Tag2,
   class Tag3
> class Triple_index_vector {
   typedef Triple_index_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::el0 el0;
   typedef typename config::el1 el1;
   typedef typename config::el2 el2;
   typedef typename config::el3 el3;
   typedef typename config::fragment fragment;
   typedef typename config::fragment_set fragment_set;
   typedef std::vector<fragment_set> storage;

   static fragment_set const& empty_set() {
      static const fragment_set fs;
      return fs;
   }

public:
   void insert(Triple const& t) {
      const std::size_t n = boost::fusion::at<Tag0>(t)();
      if( n >= v_.size() ) v_.resize(n + 1);
      v_[n].insert(
               fragment(
                        boost::fusion::at<Tag1>(t),
                        boost::fusion::at<Tag2>(t),
                        boost::fusion::at<Tag3>(t)
               )
      );
   }

   fragment_set const& operator[] (el0 const& id) const {
      if( id() >= v_.size() ) return empty_set();
      return v_[id()];
   }

private:
   storage v_;
};


}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_MAP_VECTOR_HPP_ */
