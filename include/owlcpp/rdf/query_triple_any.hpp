/** @file "/owlcpp/include/owlcpp/rdf/query_triple_any.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef QUERY_TRIPLE_ANY_HPP_
#define QUERY_TRIPLE_ANY_HPP_

#include <boost/range/concepts.hpp>
#include <boost/utility/enable_if.hpp>
#include "boost/range/any_range.hpp"

#include "owlcpp/rdf/config.hpp"

namespace owlcpp{
class Triple_store;
struct Triple;
class Node_id;
class Doc_id;

typedef boost::any_range<
         Triple, boost::single_pass_traversal_tag, Triple, std::ptrdiff_t
         > triple_any_range;

/**@brief search triples without using templates
*******************************************************************************/
OWLCPP_RDF_DECL triple_any_range find_triple(
         Triple_store const& ts,
         Node_id const* subj = 0,
         Node_id const* pred = 0,
         Node_id const* obj = 0,
         Doc_id const* doc = 0
);

}//namespace owlcpp
#endif /* QUERY_TRIPLE_ANY_HPP_ */
