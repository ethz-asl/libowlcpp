/** @file "/owlcpp/lib/rdf/iri_store_owl.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/iri_store_owl.hpp"

#include "boost/mpl/size.hpp"
#include "boost/mpl/for_each.hpp"

#include "owlcpp/rdf/detail/iri_tag_inserter.hpp"
#include "type_vector.hpp"

namespace owlcpp {

/**
*******************************************************************************/
Iri_store_owl::Iri_store_owl()
: Iri_store(boost::mpl::size<terms::mpl_vector_namespaces_all_t>::type::value)
{
   detail::Iri_tag_inserter iti(*this);
   boost::mpl::for_each<terms::mpl_vector_namespaces_all_t>(iti);
}

}//namespace owlcpp
