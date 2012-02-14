/** @file "/owlcpp/lib/rdf/node_map.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_map.hpp"

namespace owlcpp {

/*
*******************************************************************************/
void Node_map::remove(const Node_id id) {
   id_index_t & id_index = nodes_.get<id_tag>();
   id_iter_t i = id_index.find(id);
   BOOST_ASSERT( i != id_index.end() );
   id_index.erase(i);
   tracker_.push(id);
}

/*
*******************************************************************************/
void Node_map::remove(Node const& node) {
   node_index_t& node_index = nodes_.get<node_tag>();
   node_iter_t i = node_index.find(node);
   if( i == node_index.end() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("removing non-existing node")
            << Err::str1_t(node.value_str())
   );
   const Node_id id = i->first;
   node_index.erase(i);
   tracker_.push(id);
}

/*
*******************************************************************************/
Node_id Node_map::insert_literal(
         std::string const& value,
         const Node_id dt,
         std::string const& lang
) {
   const Node node(terms::N_empty::id(), value);
   node_index_t const& n_index = nodes_.get<node_tag>();
   const node_iter_t n_iter = n_index.find(node);
   if( n_iter == n_index.end() ) return insert_literal_private(node, dt, lang);
   const Node_id id = n_iter->first;
   Node_id const* dtp = datatype(id);
   Node_id dt0 = dtp ? *dtp : terms::T_empty_::id();
   if( dt != dt0 ) return insert_literal_private(node, dt, lang);
   if( lang != language(id) ) return insert_literal_private(node, dt, lang);
   return id;
}


}//namespace owlcpp
