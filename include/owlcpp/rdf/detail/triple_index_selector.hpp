/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_selector.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_SELECTOR_HPP_
#define TRIPLE_INDEX_SELECTOR_HPP_
#include "boost/mpl/distance.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/max_element.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief find best index for the query
*******************************************************************************/
template<
   class Indexes,
   class QSubj, class QPred, class QObj, class QDoc
> struct Index_selector {

   template<class Index1, class Index2> struct less
            : boost::mpl::bool_<(
                     Index1::template query<QSubj,QPred,QObj,QDoc>::efficiency <
                     Index2::template query<QSubj,QPred,QObj,QDoc>::efficiency
            )>
            {};

   typedef typename boost::mpl::max_element<
            Indexes,
            less<boost::mpl::_1, boost::mpl::_2>
   >::type iter;

   typedef typename boost::mpl::distance<
            typename boost::mpl::begin<Indexes>::type,
            iter
            >::type index;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_SELECTOR_HPP_ */
