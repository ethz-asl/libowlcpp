/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_selector.hpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_SELECTOR_HPP_
#define TRIPLE_INDEX_SELECTOR_HPP_
#include "boost/mpl/at.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/int.hpp"
#include "boost/mpl/less.hpp"
#include "boost/mpl/max_element.hpp"
#include "boost/mpl/negate.hpp"
#include "boost/mpl/plus.hpp"
#include "boost/mpl/vector_c.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Estimate of how well a given triple index would perform a search
@details

@tparam Tags sequence of integer types showing how triples are sorted by index
,e.g., for index that sorts triples by object, by docID, by subject, and then
by predicate, the sequence is
@code mpl::vector4<Obj_tag, Doc_tag, Subj_tag, Pred_tag> @endcode
@tparam QBSig boolean signature of a query, a sequence of 4 booleans indicating,
for each triple element, whether explicit match was requested,
e.g., for a query @code any, any, Node_id, Doc_id @endcode
boolean signature is @code mpl::vector4_c<bool,0,0,1,1>@endcode
*******************************************************************************/
template<
   class Tags,
   class QBSig
> class Search_efficiency {

   typedef boost::mpl::int_<100> key1_value;
   typedef boost::mpl::int_<10>  key2_value;
   typedef boost::mpl::int_<1>   key3_value;

   /** relative diversity of elements in each triple position */
   typedef boost::mpl::vector4_c<int,4, 2, 3, 1> diversity;

   /**  */
   typedef typename boost::mpl::if_<
            typename boost::mpl::at<
               QBSig,
               typename boost::mpl::front<Tags>::type
            >::type,
            boost::mpl::plus<
               key1_value,
               typename boost::mpl::at<
                  diversity,
                  typename boost::mpl::front<Tags>::type
               >::type
            >,
            boost::mpl::negate<
               typename boost::mpl::at<
                  diversity,
                  typename boost::mpl::front<Tags>::type
               >::type
            >
   >::type main_key_score;

   typedef typename boost::mpl::if_<
            typename boost::mpl::at<
               QBSig,
               typename boost::mpl::at_c<Tags,1>::type
            >::type,
            boost::mpl::plus<
               key2_value,
               typename boost::mpl::if_<
                  typename boost::mpl::at<
                     QBSig,
                     typename boost::mpl::at_c<Tags,2>::type
                  >::type,
                  key3_value,
                  boost::mpl::int_<0>
               >::type
            >,
            boost::mpl::int_<0>
   >::type sec_keys_score;


public:
   typedef typename boost::mpl::plus<main_key_score, sec_keys_score>::type type;
   static const int value = type::value;
};

template<
   class Indexes,
   class QBSig
> struct Index_selector {

   template<class I1, class I2> struct less
            : boost::mpl::less<
              typename Search_efficiency<typename I1::sort_order, QBSig>::type,
              typename Search_efficiency<typename I2::sort_order, QBSig>::type
              >
            {};

   typedef typename boost::mpl::max_element<
            Indexes,
            less<boost::mpl::_1, boost::mpl::_2>
   >::type iter;

//   typedef typename boost::mpl::deref<iter>::type index;

   typedef typename boost::mpl::distance<
            typename boost::mpl::begin<Indexes>::type,
            iter
            >::type index;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_SELECTOR_HPP_ */
