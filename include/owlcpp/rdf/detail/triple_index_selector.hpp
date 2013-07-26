/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_selector.hpp"
part of owlcpp2 project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_SELECTOR_HPP_
#define TRIPLE_INDEX_SELECTOR_HPP_
#include "boost/mpl/fold.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/less.hpp"
#include "boost/mpl/max_element.hpp"
#include "boost/mpl/transform_view.hpp"
#include "boost/mpl/plus.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/vector.hpp"
#include "boost/mpl/vector_c.hpp"
#include "boost/type_traits/is_same.hpp"

#include "owlcpp/rdf/detail/map_triple_tags.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Estimate of how well a given triple index would perform a search
@tparam Tags vector of integers showing how triples are sorted by index
,e.g., @code mpl::vector4<Obj_tag, Doc_tag, Subj_tag, Pred_tag> @endcode
@tparam QArgs sequence of query element types, e.g.,
@code mpl::vector4<any, any, Node_id, Doc_id> @endcode
@tparam Prefs sequence of relative search efficiencies for each triple element,
e.g., query<any, any, Node_id, Doc_id> is faster using
index<Obj_tag, Doc_tag, Subj_tag, Pred_tag> than using
index<Doc_tag, Obj_tag, Subj_tag, Pred_tag>
*******************************************************************************/
template<
   class Tags,
   class QArgs,
   class Prefs = boost::mpl::vector4_c<int,4,2,3,1>
> class Search_efficiency {
   typedef boost::mpl::vector4_c<int,0, 1, 2, 3> count;
   typedef boost::mpl::vector4_c<int,100, 10, 1, 0> scores;
public:
   typedef typename boost::mpl::fold<
            count,
            boost::mpl::int_<0>,
            boost::mpl::if_<
               boost::is_same<
                  boost::mpl::at<Triple, boost::mpl::at<Tags, boost::mpl::_2> >,
                  boost::mpl::at<QArgs, boost::mpl::at<Tags, boost::mpl::_2> >
                  >,
               boost::mpl::plus<
                  boost::mpl::_1,
                  boost::mpl::times<
                     boost::mpl::at<scores, boost::mpl::_2>,
                     boost::mpl::at<Prefs, boost::mpl::at<Tags, boost::mpl::_2> >
                  >
               >,
               boost::mpl::_1
            >
            >::type type;
   static const int value = type::value;
};

template<
   class Indexes,
   class QArgs
> struct Index_selector {

   template<class I1, class I2> struct less
            : boost::mpl::less<
              typename Search_efficiency<typename I1::sort_order, QArgs>::type,
              typename Search_efficiency<typename I2::sort_order, QArgs>::type
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
