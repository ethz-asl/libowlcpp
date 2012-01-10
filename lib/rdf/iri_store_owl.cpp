/** @file "/owlcpp/lib/rdf/iri_store_owl.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/iri_store_owl.hpp"

#include "boost/mpl/for_each.hpp"

#include "iri_tag_vector_owl.hpp"
#include "owlcpp/rdf/detail/iri_tag_inserter.hpp"

namespace owlcpp {

/**
*******************************************************************************/
Iri_store_owl::Iri_store_owl() {
   detail::Iri_tag_inserter iti(*this);
   boost::mpl::for_each<terms::mpl_vector_iris_owl_t>(iti);
}

}//namespace owlcpp
