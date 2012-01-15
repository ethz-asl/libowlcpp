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
   store_.insert( std::make_pair(id, node) );
   tracker_.ensure_min(id);
}

/*
*******************************************************************************/
void Node_store::remove(const id_type id) {
   id_index_t & id_index = store_.get<id_tag>();
   id_iter_t i = id_index.find(id);
   if( i == id_index.end() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("removing non-existing node ID")
            << Err::int1_t(id())
   );
   id_index.erase(id);
   tracker_.push(id);
}

/*
*******************************************************************************/
void Node_store::remove(Node const& node) {
   node_index_t& node_index = store_.get<node_tag>();
   node_iter_t i = node_index.find(node);
   if( i == node_index.end() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("removing non-existing node")
            << Err::str1_t(node.value_str())
   );
   const id_type id = i->first;
   node_index.erase(i);
   tracker_.push(id);
}

}//namespace owlcpp
