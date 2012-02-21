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
   explicit Tag_inserter(Map& map) : map_(&map) {}

   template<class T> void operator()(T const&) const {
      map_->insert_tag(T());
   }

   mutable Map* map_;
};
}//namespace anonymous

/*
*******************************************************************************/
void Nodes_owl::operator()(Iri_map_base& map) const {
   Tag_inserter<Iri_map_base> ti(map);
   boost::mpl::for_each<terms::mpl_vector_iris_t>(ti);
}

/*
*******************************************************************************/
void Nodes_owl::operator()(Node_map_base& map) const {
   Tag_inserter<Node_map_base> ti(map);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(ti);
}

}//namespace owlcpp
