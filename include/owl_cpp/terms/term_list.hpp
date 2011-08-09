/** @file "/owl_cpp/include/owl_cpp/terms/term_list.hpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TERM_LIST_HPP_
#define TERM_LIST_HPP_

/**@brief OWL terms in RDFS namespace
*******************************************************************************/
#define OWLCPP_TERMS_RDFS \
      ((rdfs)(comment)) \
      ((rdfs)(Datatype)) \
      ((rdfs)(domain)) \
      ((rdfs)(isDefinedBy)) \
      ((rdfs)(label)) \
      ((rdfs)(range)) \
      ((rdfs)(subClassOf)) \
      ((rdfs)(subPropertyOf)) \
/*
      ((rdfs)(Resource)) \
      ((rdfs)(Literal)) \
      ((rdfs)(Class)) \
      ((rdfs)(member)) \
      ((rdfs)(Container)) \
      ((rdfs)(ContainerMembershipProperty)) \
      ((rdfs)(seeAlso)) \
*/

/**@brief OWL terms in RDF namespace
*******************************************************************************/
#define OWLCPP_TERMS_RDF \
      ((rdf)(first)) \
      ((rdf)(nil)) \
      ((rdf)(Property)) \
      ((rdf)(rest)) \
      ((rdf)(type)) \
/*
      ((rdf)(XMLLiteral)) \
      ((rdf)(List)) \
      ((rdf)(Statement)) \
      ((rdf)(subject)) \
      ((rdf)(predicate)) \
      ((rdf)(object)) \
      ((rdf)(Seq)) \
      ((rdf)(Bag)) \
      ((rdf)(Alt)) \
      ((rdf)(_1)) \
      ((rdf)(_2)) \
      ((rdf)(value)) \
*/

/**@brief OWL 2 RDF-Based Vocabulary
 http://www.w3.org/TR/2009/REC-owl2-rdf-based-semantics-20091027/
*******************************************************************************/
#define OWLCPP_TERMS_OWL1 \
      ((owl)(AllDifferent)) \
      ((owl)(AllDisjointClasses)) \
      ((owl)(AllDisjointProperties)) \
      ((owl)(allValuesFrom)) \
      ((owl)(annotatedProperty)) \
      ((owl)(annotatedSource)) \
      ((owl)(annotatedTarget)) \
      ((owl)(Annotation)) \
      ((owl)(AnnotationProperty)) \
      ((owl)(assertionProperty)) \
      ((owl)(AsymmetricProperty)) \
      ((owl)(Axiom)) \
      ((owl)(backwardCompatibleWith)) \
      ((owl)(bottomDataProperty)) \
      ((owl)(bottomObjectProperty)) \
      ((owl)(cardinality)) \
      ((owl)(Class)) \
      ((owl)(complementOf)) \
      ((owl)(DataRange)) \
      ((owl)(datatypeComplementOf)) \
      ((owl)(DatatypeProperty)) \
      ((owl)(deprecated)) \
      ((owl)(DeprecatedClass)) \
      ((owl)(DeprecatedProperty)) \
      ((owl)(differentFrom)) \
      ((owl)(disjointUnionOf)) \
      ((owl)(disjointWith)) \
      ((owl)(distinctMembers)) \
      ((owl)(equivalentClass)) \
      ((owl)(equivalentProperty)) \
      ((owl)(FunctionalProperty)) \
      ((owl)(hasKey)) \
      ((owl)(hasSelf)) \
      ((owl)(hasValue)) \
      ((owl)(imports)) \
      ((owl)(incompatibleWith)) \
      ((owl)(intersectionOf)) \
      ((owl)(InverseFunctionalProperty)) \
      ((owl)(inverseOf)) \
/* */

#define OWLCPP_TERMS_OWL2 \
      ((owl)(IrreflexiveProperty)) \
      ((owl)(maxCardinality)) \
      ((owl)(maxQualifiedCardinality)) \
      ((owl)(members)) \
      ((owl)(minCardinality)) \
      ((owl)(minQualifiedCardinality)) \
      ((owl)(NamedIndividual)) \
      ((owl)(NegativePropertyAssertion)) \
      ((owl)(Nothing)) \
      ((owl)(ObjectProperty)) \
      ((owl)(onClass)) \
      ((owl)(onDataRange)) \
      ((owl)(onDatatype)) \
      ((owl)(oneOf)) \
      ((owl)(onProperties)) \
      ((owl)(onProperty)) \
      ((owl)(Ontology)) \
      ((owl)(OntologyProperty)) \
      ((owl)(priorVersion)) \
      ((owl)(propertyChainAxiom)) \
      ((owl)(propertyDisjointWith)) \
      ((owl)(qualifiedCardinality)) \
      ((owl)(ReflexiveProperty)) \
      ((owl)(Restriction)) \
      ((owl)(sameAs)) \
      ((owl)(someValuesFrom)) \
      ((owl)(sourceIndividual)) \
      ((owl)(SymmetricProperty)) \
      ((owl)(targetIndividual)) \
      ((owl)(targetValue)) \
      ((owl)(Thing)) \
      ((owl)(topDataProperty)) \
      ((owl)(topObjectProperty)) \
      ((owl)(TransitiveProperty)) \
      ((owl)(unionOf)) \
      ((owl)(versionInfo)) \
      ((owl)(versionIRI)) \
      ((owl)(withRestrictions)) \
/* */

/**@brief OWL terms in XSD namespace
*******************************************************************************/
#define OWLCPP_TERMS_XSD \
/* */

/**@brief miscellaneous terms
*******************************************************************************/
#define OWLCPP_TERMS_MISC \
      (()(generic)()) \
/* */

/**@brief Combined set of supported OWL terms
@details Define sequences of standard OWL terms.
The sequences are used for generating small Tag types and typelists.
Each term is defined by a sequence of 2 or 3 elements
of which the first one refers to IRI prefix defined in "uri_list.hpp".
The name of the type is formed by concatenating first and second elements.
The last element---either second or third---is used as string value of the term.
*******************************************************************************/
#define OWLCPP_TERMS_ALL \
      OWLCPP_TERMS_MISC \
      OWLCPP_TERMS_OWL1 \
      OWLCPP_TERMS_OWL2 \
      OWLCPP_TERMS_RDF \
      OWLCPP_TERMS_RDFS \
      OWLCPP_TERMS_XSD \
/* */

/**@brief Visitable tags: subset of OWLCPP_TERMS_ALL
@details These tags are used in Triple_std<Tag> types, which are passed to
reasoner using visitor pattern.
The meaning of a triple is usually determined by its predicate.
Therefore most triples carry tags that come from their predicate nodes.
However, triples with rdf:type predicate take tags from their object nodes.
Visitor implements a method for each of the Tags
e.g. void Visit(const Triple_std<ot::T_owl_IrreflexiveProperty>&);
*******************************************************************************/
#define OWLCPP_TERMS_VISITABLE \
      (()(generic)()) \
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


#endif /* TERM_LIST_HPP_ */
