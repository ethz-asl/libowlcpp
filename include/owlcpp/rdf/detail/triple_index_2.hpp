/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_2.hpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_2_HPP_
#define TRIPLE_INDEX_2_HPP_
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/contains.hpp"
#include "boost/mpl/remove.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/mpl/sort.hpp"
#include "owlcpp/rdf/detail/map_triple_tags.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/detail/fragment_set.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief 
*******************************************************************************/
template<
//   template<class,class,class,class> class Map,
   class Tag0,
   class Tag1,
   class Tag2,
   class Tag3
> class Triple_index {
   typedef boost::mpl::vector4<Tag0,Tag1,Tag2,Tag3> tags;
   BOOST_MPL_ASSERT((
            boost::mpl::equal<typename boost::mpl::sort<tags>::type, triple_tags>
   ));

   typedef Fragment_set<
            typename boost::mpl::at<Triple, Tag1>::type,
            typename boost::mpl::at<Triple, Tag2>::type,
            typename boost::mpl::at<Triple, Tag3>::type
            >
   fragment_set;
   typedef typename fragment_set::value_type fragment;
   typedef std::vector<fragment_set> stor;

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

private:
   stor v_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_2_HPP_ */
