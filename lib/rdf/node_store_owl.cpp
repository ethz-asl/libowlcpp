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
#include "owlcpp/rdf/query_iris.hpp"

namespace owlcpp{

/*
*******************************************************************************/
Node_store_owl::Node_store_owl() {
   detail::Node_tag_inserter nti(store_);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti);
}

/*
*******************************************************************************/
Node_id Node_store_owl::insert(Node const& node) {
   Node_id const* id = find(node);
   if( id ) return *id;
   if( is_std_owl(node.ns_id()) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("inserting unknown term into standard OWL namespace")
            << Err::str1_t( node.value_str() )
            << Err::int1_t( node.ns_id()() )
   );
   return store_.insert(node);
}

}//namespace owlcpp
