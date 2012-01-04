/** @file "/owlcpp/lib/rdf/node_store.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_store.hpp"
#include "type_vector.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/for_each.hpp"

namespace owlcpp {

/**
*******************************************************************************/
Node_store Node_store::owl() {
   Node_store ns(
            boost::mpl::size<terms::mpl_vector_terms_other_t>::type::value +
            boost::mpl::size<terms::mpl_vector_terms_rdfs_t>::type::value +
            boost::mpl::size<terms::mpl_vector_terms_rdf_t>::type::value +
            boost::mpl::size<terms::mpl_vector_terms_owl1_t>::type::value +
            boost::mpl::size<terms::mpl_vector_terms_owl2_t>::type::value
   );

   ns.iris_.reset( new Iri_store( (terms::mpl_vector_namespaces_all_t()) ) );
   Store_node_tags snt(ns);
   boost::mpl::for_each<terms::mpl_vector_terms_other_t>(snt);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(snt);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(snt);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(snt);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(snt);
   return ns;
}

}//namespace owlcpp
