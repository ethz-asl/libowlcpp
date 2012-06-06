/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_query_tags.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_QUERY_TAGS_HPP_
#define TRIPLE_QUERY_TAGS_HPP_
#include "boost/mpl/size_t.hpp"

namespace owlcpp{ namespace query_detail{
namespace mpl = boost::mpl;

struct Subj_tag : public mpl::size_t<0> {};
struct Pred_tag : public mpl::size_t<1> {};
struct Obj_tag  : public mpl::size_t<2> {};
struct Doc_tag  : public mpl::size_t<3> {};

}//namespace query_detail
}//namespace owlcpp
#endif /* TRIPLE_QUERY_TAGS_HPP_ */
