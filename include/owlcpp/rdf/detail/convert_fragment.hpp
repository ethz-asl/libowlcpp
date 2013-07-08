/** @file "/owlcpp/include/owlcpp/rdf/detail/convert_fragment.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef CONVERT_FRAGMENT_HPP_
#define CONVERT_FRAGMENT_HPP_
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/fusion/sequence/intrinsic/at_c.hpp"
#include "owlcpp/rdf/detail/map_triple_tags.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Convert triple to fragment and v.v.
*******************************************************************************/
template<
   class Tag0, /**< numerical tag indicating which triple element is indexed first */
   class Tag1, /**< numerical tag indicating which triple element is indexed second */
   class Tag2, /**< numerical tag indicating which triple element is indexed third */
   class Tag3  /**< numerical tag indicating which triple element is indexed fourth */
> struct Convert_fragment {
   typedef typename boost::mpl::at<Triple, Tag0>::type el0;
   typedef typename boost::mpl::at<Triple, Tag1>::type el1;
   typedef typename boost::mpl::at<Triple, Tag2>::type el2;
   typedef typename boost::mpl::at<Triple, Tag3>::type el3;
   typedef boost::fusion::vector3<el1,el2,el3> fragment;

   static el0 get_index(Triple const& t) {return boost::fusion::at<Tag0>(t);}

   static fragment get_fragment(Triple const& t) {
      return fragment(
               boost::fusion::at<Tag1>(t),
               boost::fusion::at<Tag2>(t),
               boost::fusion::at<Tag3>(t)
      );
   }

   static Triple get_triple(const el0 i, fragment const& f) {
      Triple t;
      boost::fusion::at<Tag0>(t) = i;
      boost::fusion::at<Tag1>(t) = boost::fusion::at_c<0>(f);
      boost::fusion::at<Tag2>(t) = boost::fusion::at_c<1>(f);
      boost::fusion::at<Tag3>(t) = boost::fusion::at_c<2>(f);
      return t;
   }
};


}//namespace map_triple_detail
}//namespace owlcpp
#endif /* CONVERT_FRAGMENT_HPP_ */
