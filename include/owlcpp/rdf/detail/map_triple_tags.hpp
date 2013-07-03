/** @file "/owlcpp/include/owlcpp/rdf/detail/map_triple_tags.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_TRIPLE_TAGS_HPP_
#define MAP_TRIPLE_TAGS_HPP_
#include "boost/fusion/adapted/struct/adapt_struct.hpp"
#include "boost/mpl/vector.hpp"

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

namespace map_triple_detail{

namespace mpl = boost::mpl;

/* Tags for query elements
*******************************************************************************/
struct Subj_tag : public mpl::size_t<0> {};
struct Pred_tag : public mpl::size_t<1> {};
struct Obj_tag  : public mpl::size_t<2> {};
struct Doc_tag  : public mpl::size_t<3> {};
typedef mpl::vector<Subj_tag, Pred_tag, Obj_tag, Doc_tag> triple_tags;

struct Main_store_tag{};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* MAP_TRIPLE_TAGS_HPP_ */
