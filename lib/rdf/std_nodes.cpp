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
#include "owlcpp/rdf/ns_map_base.hpp"

namespace owlcpp {

namespace{

struct Node_tag_inserter {
   explicit Node_tag_inserter(Node_map_base& map) : map_(&map) {}

   template<class T> void operator()(T const&) const {
      map_->insert_tag(T());
   }

   mutable Node_map_base* map_;
};

struct Ns_tag_inserter {
   explicit Ns_tag_inserter(Ns_map_base& map) : map_(&map) {}

   template<class T> void operator()(T const&) const {
      map_->insert(T::id(), T::iri());
      map_->set_prefix(T::id(), T::prefix());
   }

   mutable Ns_map_base* map_;
};
}//namespace anonymous

/*
*******************************************************************************/
void Nodes_owl::operator()(Ns_map_base& map) const {
   Ns_tag_inserter ti(map);
   boost::mpl::for_each<terms::mpl_vector_iris_t>(ti);
}

/*
*******************************************************************************/
void Nodes_owl::operator()(Node_map_base& map) const {
   Node_tag_inserter ti(map);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(ti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(ti);
}

}//namespace owlcpp
