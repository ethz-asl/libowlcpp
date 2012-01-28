/** @file "/owlcpp/lib/rdf/triple_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/triple_store.hpp"

#include "boost/mpl/for_each.hpp"

#include "node_tag_vector_owl.hpp"
#include "owlcpp/rdf/detail/node_tag_inserter.hpp"

namespace owlcpp {

/**
*******************************************************************************/
Triple_store::Triple_store() {
   detail::Node_tag_inserter nti(node_);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti);

}

}//namespace owlcpp
