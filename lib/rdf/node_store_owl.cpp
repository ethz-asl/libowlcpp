/** @file "/owlcpp/lib/rdf/node_store_owl.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_store_owl.hpp"

#include "boost/mpl/size.hpp"
#include "boost/mpl/for_each.hpp"

#include "owlcpp/rdf/iri_store_owl.hpp"
#include "type_vector.hpp"

namespace owlcpp{ namespace detail{
class Node_tags_inserter {
public:
   Node_tags_inserter(Node_store_owl& store) : nstor_(store) {}
   template<class T> void operator()(const T&) const {
      nstor_.insert( T::id(), Node(T::ns_type::id(), T::name()) );
   }
private:
   mutable Node_store_owl& nstor_;
};
}//namespace detail

/**
*******************************************************************************/
Node_store_owl::Node_store_owl()
: Node_store(
         Iri_store_owl(),
         boost::mpl::size<terms::mpl_vector_terms_other_t>::type::value +
         boost::mpl::size<terms::mpl_vector_terms_rdfs_t>::type::value +
         boost::mpl::size<terms::mpl_vector_terms_rdf_t>::type::value +
         boost::mpl::size<terms::mpl_vector_terms_owl1_t>::type::value +
         boost::mpl::size<terms::mpl_vector_terms_owl2_t>::type::value
)
{
   detail::Node_tags_inserter nti(*this);
   boost::mpl::for_each<terms::mpl_vector_terms_other_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti);
}

}//namespace owlcpp
