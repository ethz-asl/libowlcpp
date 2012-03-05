/** @file "/owlcpp/include/owlcpp/terms/node_tags_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TAGS_OWL_HPP_
#define NODE_TAGS_OWL_HPP_

/**@brief Standard OWL nodes in RDFS namespace
@details ((standard namespace IRI prefix)(node name))
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
         ((xsd)(string)) \
         ((xsd)(boolean)) \
         ((xsd)(decimal)) \
         ((xsd)(float)) \
         ((xsd)(double)) \
         ((xsd)(duration)) \
         ((xsd)(dateTime)) \
         ((xsd)(time)) \
         ((xsd)(date)) \
         ((xsd)(gYearMonth)) \
         ((xsd)(gYear)) \
         ((xsd)(gMonthDay)) \
         ((xsd)(gDay)) \
         ((xsd)(gMonth)) \
         ((xsd)(hexBinary)) \
         ((xsd)(base64Binary)) \
         ((xsd)(anyURI)) \
         ((xsd)(QName)) \
         ((xsd)(NOTATION)) \
         ((xsd)(normalizedString)) \
         ((xsd)(token)) \
         ((xsd)(language)) \
         ((xsd)(NMTOKEN)) \
         ((xsd)(Name)) \
         ((xsd)(NCName)) \
         ((xsd)(ID)) \
         ((xsd)(IDREF)) \
         ((xsd)(IDREFS)) \
         ((xsd)(ENTITY)) \
         ((xsd)(ENTITIES)) \
         ((xsd)(integer)) \
         ((xsd)(nonPositiveInteger)) \
         ((xsd)(negativeInteger)) \
         ((xsd)(long)) \
         ((xsd)(int)) \
         ((xsd)(short)) \
         ((xsd)(byte)) \
         ((xsd)(nonNegativeInteger)) \
         ((xsd)(unsignedLong)) \
         ((xsd)(unsignedInt)) \
         ((xsd)(unsignedShort)) \
         ((xsd)(unsignedByte)) \
         ((xsd)(positiveInteger)) \
/* */

/**@brief Combined set of supported OWL terms
@details Define sequence of standard OWL terms.
The sequences are used for generating small Tag types and typelists.
Each term is defined by a sequence of 2 or 3 elements
of which the first one refers to IRI prefix defined in "uri_list.hpp".
The name of the type is formed by concatenating first and second elements.
The last element---either second or third---is used as string value of the term.
*******************************************************************************/
#define OWLCPP_TERMS_ALL \
      OWLCPP_TERMS_OWL1 \
      OWLCPP_TERMS_OWL2 \
      OWLCPP_TERMS_RDF \
      OWLCPP_TERMS_RDFS \
      OWLCPP_TERMS_XSD \
/* */

#include <string>
#include "owlcpp/terms/term_macro.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{ namespace terms{

/**@brief
@code
struct T_empty_ {
   typedef N_empty ns_type;
   typedef ::owlcpp::Node_id id_type;
   static const unsigned index = 0;
   static std::string const & name() {
      static const std::string str = std::string("");
      return str;
   }
   static id_type id() {return id_type(index);}
}; @endcode

*******************************************************************************/
OWLCPP_GENERATE_NODE_TAGS(OWLCPP_TERMS_ALL, 5)

}//namespace terms
}//namespace owlcpp
#endif /* NODE_TAGS_OWL_HPP_ */
