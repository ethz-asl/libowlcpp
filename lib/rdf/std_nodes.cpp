/** @file "/owlcpp/lib/rdf/std_nodes.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/std_nodes.hpp"

#include "boost/mpl/for_each.hpp"

#include "iri_tag_vector.hpp"
#include "node_tag_vector_system.hpp"
#include "node_tag_vector_owl.hpp"
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/rdf/iri_map_base.hpp"

namespace owlcpp {

namespace{

template<class Map> struct Tag_inserter {
   typedef typename Map::id_type id_type;
   explicit Tag_inserter(Map& map, id_type& next_id)
   : map_(&map), next_id_(next_id) {}

   template<class T> void operator()(T const&) const {
      map_->insert_tag(T());
      if( next_id_ <= T::id() ) next_id_ = id_type(T::id()() + 1);
   }

   mutable Map* map_;
   mutable id_type& next_id_;
};
}//namespace anonymous

/*
*******************************************************************************/
Ns_id Nodes_system::operator()(Iri_map_base& map) const {
   Ns_id iid(0);
   Tag_inserter<Iri_map_base> ti(map, iid);
   ti(terms::N_empty());
   ti(terms::N_blank());
   return iid;
}

/*
*******************************************************************************/
Node_id Nodes_system::operator()(Node_map_base& map) const {
   Node_id nid(0);
   Tag_inserter<Node_map_base> ti(map, nid);
   ti(terms::T_empty_());
   return nid;
}

/*
*******************************************************************************/
Ns_id Nodes_owl::operator()(Iri_map_base& map) const {
   Ns_id iid(0);
   Tag_inserter<Iri_map_base> ti(map, iid);
   boost::mpl::for_each<terms::mpl_vector_iris_t>(ti);
   return iid;
}

/*
*******************************************************************************/
Node_id Nodes_owl::operator()(Node_map_base& map) const {
   Node_id nid(0);
   Tag_inserter<Node_map_base> ti(map, nid);
   boost::mpl::for_each<terms::mpl_vector_terms_system_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(ti);
   return nid;
}

}//namespace owlcpp
