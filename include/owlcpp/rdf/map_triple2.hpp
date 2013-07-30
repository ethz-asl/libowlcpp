/** @file "/owlcpp/include/owlcpp/rdf/map_triple2.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef MAP_TRIPLE2_HPP_
#define MAP_TRIPLE2_HPP_
#include "boost/fusion/algorithm/iteration/for_each.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/include/mpl.hpp"
#include "boost/fusion/sequence/intrinsic/front.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/push_back.hpp"
#include "owlcpp/rdf/detail/triple_index_2.hpp"
#include "owlcpp/rdf/map_triple_config.hpp"

namespace owlcpp{

/**@brief Store, index, and search RDF triples
*******************************************************************************/
template<
   class Config = map_triple_detail::index_config_default
>
class Map_triple {
   typedef typename boost::mpl::fold<
            Config,
            boost::fusion::vector<>,
            boost::mpl::push_back<
               boost::mpl::_1,
               map_triple_detail::Triple_index_selector<boost::mpl::_2>
            >
   >::type store;

   typedef typename boost::mpl::front<store>::type index1;

public:
   typedef typename index1::const_iterator const_iterator;
   typedef typename index1::iterator iterator;

   Map_triple() : store_(), size_(0) {}

   std::size_t size() const {return size_;}
   bool empty() const {return ! size_;}
   const_iterator begin() const {return front(store_).begin();}
   const_iterator end() const {return front(store_).end();}

   void insert(Triple const& t) {
      bool inserted;
      map_triple_detail::Insert insert(t, inserted);
      boost::fusion::for_each(store_, insert);
      if( inserted ) ++size_;
   }

   void erase(Triple const& t) {
      map_triple_detail::Erase erase(t);
      boost::fusion::for_each(store_, erase);
      --size_;
   }

private:
   store store_;
   std::size_t size_;
};

}//namespace owlcpp
#endif /* MAP_TRIPLE2_HPP_ */
