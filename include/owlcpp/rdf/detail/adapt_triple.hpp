/** @file "/owlcpp/include/owlcpp/rdf/detail/adapt_triple.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef ADAPT_TRIPLE_HPP_
#define ADAPT_TRIPLE_HPP_

#include "boost/fusion/adapted/struct/adapt_struct.hpp"
#include "owlcpp/rdf/triple.hpp"

BOOST_FUSION_ADAPT_STRUCT(
         owlcpp::Triple,
         (owlcpp::Node_id, subj_)
         (owlcpp::Node_id, pred_)
         (owlcpp::Node_id, obj_)
         (owlcpp::Doc_id, doc_)
)

#endif /* ADAPT_TRIPLE_HPP_ */
