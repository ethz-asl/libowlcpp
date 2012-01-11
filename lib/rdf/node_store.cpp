/** @file "/owlcpp/lib/rdf/node_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_store.hpp"

#include "boost/mpl/for_each.hpp"

#include "node_tag_vector_system.hpp"
#include "owlcpp/rdf/detail/node_tag_inserter.hpp"

namespace owlcpp {

/*
*******************************************************************************/
Node_store::Node_store() : tracker_(), store_() {
   detail::Node_tag_inserter nti(*this);
   boost::mpl::for_each<terms::mpl_vector_nodes_system_t>(nti);
}

/*
*******************************************************************************/
void Node_store::insert(const Node_id id, Node const& node) {
   BOOST_ASSERT(
            store_.get<id_tag>().find(id) == store_.get<id_tag>().end()
   );
   BOOST_ASSERT(
            store_.get<node_tag>().find(node) == store_.get<node_tag>().end()
   );
   store_.insert( boost::make_tuple(id, node) );
   tracker_.ensure_min(id);
}

}//namespace owlcpp
