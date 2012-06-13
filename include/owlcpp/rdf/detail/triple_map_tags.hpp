/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_map_tags.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_TAGS_HPP_
#define TRIPLE_MAP_TAGS_HPP_
#include "boost/fusion/adapted/struct/adapt_struct.hpp"

#include "owlcpp/rdf/triple.hpp"

BOOST_FUSION_ADAPT_STRUCT(
         owlcpp::Triple,
         (owlcpp::Node_id, subj_)
         (owlcpp::Node_id, pred_)
         (owlcpp::Node_id, obj_)
         (owlcpp::Doc_id, doc_)
)

namespace owlcpp{

struct any;

namespace detail{

namespace mpl = boost::mpl;

/* Tags for query elements
*******************************************************************************/
struct Subj_tag : public mpl::size_t<0> {};
struct Pred_tag : public mpl::size_t<1> {};
struct Obj_tag  : public mpl::size_t<2> {};
struct Doc_tag  : public mpl::size_t<3> {};

struct Main_store_tag{};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_MAP_TAGS_HPP_ */
