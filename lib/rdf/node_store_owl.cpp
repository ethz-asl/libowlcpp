/** @file "/owlcpp/lib/rdf/node_store_owl.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_store_owl.hpp"

#include "boost/mpl/for_each.hpp"

#include "node_tag_vector_owl.hpp"
#include "owlcpp/rdf/detail/node_tag_inserter.hpp"

namespace owlcpp{

/**
*******************************************************************************/
Node_store_owl::Node_store_owl() : Node_store() {
   detail::Node_tag_inserter nti(*this);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti);
}

}//namespace owlcpp
