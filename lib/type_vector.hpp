/** @file "/owlcpp/lib/type_vector.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TYPE_VECTOR_HPP_
#define TYPE_VECTOR_HPP_
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "type_vector_macro.hpp"
#include "rdf/iri_tag_vector.hpp"
#include "rdf/node_tag_vector_system.hpp"
#include "rdf/node_tag_vector_owl.hpp"

#define OWLCPP_TERMS_VISITABLE \
      ((empty)()) \
      ((owl)(AllDifferent)) \
      ((owl)(AllDisjointClasses)) \
      ((owl)(AllDisjointProperties)) \
      ((owl)(allValuesFrom)) \
      ((owl)(AsymmetricProperty)) \
      ((owl)(cardinality)) \
      ((owl)(Class)) \
      ((owl)(complementOf)) \
      ((owl)(datatypeComplementOf)) \
      ((owl)(differentFrom)) \
      ((owl)(disjointUnionOf)) \
      ((owl)(disjointWith)) \
      ((owl)(equivalentClass)) \
      ((owl)(equivalentProperty)) \
      ((owl)(FunctionalProperty)) \
      ((owl)(hasKey)) \
      ((owl)(hasSelf)) \
      ((owl)(hasValue)) \
      ((owl)(intersectionOf)) \
      ((owl)(InverseFunctionalProperty)) \
      ((owl)(inverseOf)) \
      ((owl)(IrreflexiveProperty)) \
      ((owl)(maxCardinality)) \
      ((owl)(maxQualifiedCardinality)) \
      ((owl)(minCardinality)) \
      ((owl)(minQualifiedCardinality)) \
      ((owl)(NamedIndividual)) \
      ((owl)(NegativePropertyAssertion)) \
      ((owl)(ObjectProperty)) \
      ((owl)(oneOf)) \
      ((owl)(propertyChainAxiom)) \
      ((owl)(propertyDisjointWith)) \
      ((owl)(qualifiedCardinality)) \
      ((owl)(ReflexiveProperty)) \
      ((owl)(sameAs)) \
      ((owl)(someValuesFrom)) \
      ((owl)(SymmetricProperty)) \
      ((owl)(TransitiveProperty)) \
      ((owl)(unionOf)) \
      ((owl)(withRestrictions)) \
      ((rdfs)(Datatype)) \
      ((rdfs)(domain)) \
      ((rdfs)(range)) \
      ((rdfs)(subClassOf)) \
      ((rdfs)(subPropertyOf)) \
/* */

#include OWLCPP_MPL_VECTORnn_HPP(OWLCPP_TERMS_VISITABLE)

namespace owlcpp{ namespace terms{

/**
@brief typelist of OWL term tags to generate visitable triple classes
*******************************************************************************/
typedef OWLCPP_TERM_MPL_VECTOR(OWLCPP_TERMS_VISITABLE)
      mpl_vector_terms_visitable_t;

}//namespace keywords
}//namespace owlcpp
#endif /* TYPE_VECTOR_HPP_ */
