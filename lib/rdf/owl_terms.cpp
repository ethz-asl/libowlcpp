/** @file "/owlcpp/lib/rdf/owl_terms.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include "owlcpp/rdf/owl_terms.hpp"

#include "boost/mpl/for_each.hpp"

#include "owlcpp/rdf/node_map.hpp"
#include "node_tag_vector_owl.hpp"
#include "owlcpp/rdf/detail/node_tag_inserter.hpp"

namespace owlcpp {

/*
*******************************************************************************/
void insert_owl_terms(Node_map& node_map) {
   detail::Node_tag_inserter nti(node_map);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti);
}

}//namespace owlcpp
