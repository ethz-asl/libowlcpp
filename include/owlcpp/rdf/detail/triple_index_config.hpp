/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_config.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_CONFIG_HPP_
#define TRIPLE_INDEX_CONFIG_HPP_
#include "owlcpp/rdf/detail/map_triple_tags.hpp"
#include "owlcpp/rdf/detail/fragment_set.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief 
*******************************************************************************/
template<
   class Tag0, class Tag1, class Tag2, class Tag3
> struct Triple_index_config {
   typedef typename boost::mpl::at<Triple, Tag0>::type el0;
   typedef typename boost::mpl::at<Triple, Tag1>::type el1;
   typedef typename boost::mpl::at<Triple, Tag2>::type el2;
   typedef typename boost::mpl::at<Triple, Tag3>::type el3;

   typedef Fragment_set<el1, el2, el3> fragment_set;
   typedef typename fragment_set::value_type fragment;
};


}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_CONFIG_HPP_ */
