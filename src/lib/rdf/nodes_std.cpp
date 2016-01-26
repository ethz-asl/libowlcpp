/** @file "/owlcpp/lib/rdf/nodes_std.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/nodes_std.hpp"
#include "boost/mpl/for_each.hpp"
#include "iri_tag_vector.hpp"
#include "node_tag_vector_system.hpp"
#include "node_tag_vector_owl.hpp"
#include "owlcpp/rdf/map_std.hpp"

namespace owlcpp{

/*
*******************************************************************************/
void Nodes_owl::operator()(Map_std& map) const {
   detail::Ns_inserter ns_ins(map);
   boost::mpl::for_each<terms::mpl_vector_iris_t>(ns_ins);

   detail::Node_inserter node_ins(map);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(node_ins);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(node_ins);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(node_ins);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(node_ins);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(node_ins);
}
}//namespace owlcpp
