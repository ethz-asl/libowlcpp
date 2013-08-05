/** @file "/owlcpp/include/owlcpp/rdf/triple_tags.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_TAGS_HPP_
#define TRIPLE_TAGS_HPP_
#include "boost/mpl/int.hpp"

namespace owlcpp{


/* Numerical tags for triple elements
*******************************************************************************/
struct Subj_tag : public boost::mpl::int_<0> {};
struct Pred_tag : public boost::mpl::int_<1> {};
struct Obj_tag  : public boost::mpl::int_<2> {};
struct Doc_tag  : public boost::mpl::int_<3> {};

}//namespace owlcpp
#endif /* TRIPLE_TAGS_HPP_ */
