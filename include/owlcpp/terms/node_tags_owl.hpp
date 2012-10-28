/** @file "/owlcpp/include/owlcpp/terms/node_tags_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TAGS_OWL_HPP_
#define NODE_TAGS_OWL_HPP_
#include <string>
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/seq/size.hpp"
#include "owlcpp/terms/term_macro.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/node_id.hpp"

/**@brief Standard OWL nodes in RDFS namespace
@details ((standard namespace IRI prefix)(node name))
*******************************************************************************/
#define OWLCPP_TERMS_RDFS_N0 5
#define OWLCPP_TERMS_RDFS \
      ((rdfs)(Class)) \
      ((rdfs)(Container)) \
      ((rdfs)(ContainerMembershipProperty)) \
      ((rdfs)(Datatype)) \
      ((rdfs)(Literal)) \
      ((rdfs)(Resource)) \
      ((rdfs)(comment)) \
      ((rdfs)(domain)) \
      ((rdfs)(isDefinedBy)) \
      ((rdfs)(label)) \
      ((rdfs)(member)) \
      ((rdfs)(range)) \
      ((rdfs)(seeAlso)) \
      ((rdfs)(subClassOf)) \
      ((rdfs)(subPropertyOf)) \
/*
*/

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
OWLCPP_GENERATE_NODE_TAGS(OWLCPP_TERMS_RDFS, OWLCPP_TERMS_RDFS_N0)

}//namespace terms
}//namespace owlcpp

/**@brief OWL terms in RDF namespace
*******************************************************************************/
#define OWLCPP_TERMS_RDF_N0 BOOST_PP_ADD(OWLCPP_TERMS_RDFS_N0, BOOST_PP_SEQ_SIZE(OWLCPP_TERMS_RDFS))
#define OWLCPP_TERMS_RDF \
      ((rdf)(List)) \
      ((rdf)(PlainLiteral)) \
      ((rdf)(Property)) \
      ((rdf)(first)) \
      ((rdf)(nil)) \
      ((rdf)(rest)) \
      ((rdf)(type)) \
      ((rdf)(XMLLiteral)) \
/*
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
namespace owlcpp{ namespace terms{
OWLCPP_GENERATE_NODE_TAGS(OWLCPP_TERMS_RDF, OWLCPP_TERMS_RDF_N0)
}}

/**@brief OWL 2 RDF-Based Vocabulary
 http://www.w3.org/TR/2009/REC-owl2-rdf-based-semantics-20091027/
*******************************************************************************/
#define OWLCPP_TERMS_OWL1_N0 BOOST_PP_ADD(OWLCPP_TERMS_RDF_N0, BOOST_PP_SEQ_SIZE(OWLCPP_TERMS_RDF))
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
namespace owlcpp{ namespace terms{
OWLCPP_GENERATE_NODE_TAGS(OWLCPP_TERMS_OWL1, OWLCPP_TERMS_OWL1_N0)
}}

#define OWLCPP_TERMS_OWL2_N0 BOOST_PP_ADD(OWLCPP_TERMS_OWL1_N0, BOOST_PP_SEQ_SIZE(OWLCPP_TERMS_OWL1))
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
      ((owl)(rational)) \
      ((owl)(real)) \
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
namespace owlcpp{ namespace terms{
OWLCPP_GENERATE_NODE_TAGS(OWLCPP_TERMS_OWL2, OWLCPP_TERMS_OWL2_N0)
}}

//TODO: map to internally stored types
/**@brief OWL terms in XSD namespace
*******************************************************************************/
#define OWLCPP_TERMS_XSD_N0 BOOST_PP_ADD(OWLCPP_TERMS_OWL2_N0, BOOST_PP_SEQ_SIZE(OWLCPP_TERMS_OWL2))
#define OWLCPP_TERMS_XSD \
         ((xsd)(anyURI)) \
         ((xsd)(base64Binary)) \
         ((xsd)(boolean)) \
         ((xsd)(byte)) \
         ((xsd)(date)) \
         ((xsd)(dateTime)) \
         ((xsd)(dateTimeStamp)) \
         ((xsd)(decimal)) \
         ((xsd)(double)) \
         ((xsd)(duration)) \
         ((xsd)(ENTITIES)) \
         ((xsd)(ENTITY)) \
         ((xsd)(float)) \
         ((xsd)(gMonthDay)) \
         ((xsd)(gYear)) \
         ((xsd)(gYearMonth)) \
         ((xsd)(gDay)) \
         ((xsd)(gMonth)) \
         ((xsd)(hexBinary)) \
         ((xsd)(ID)) \
         ((xsd)(IDREF)) \
         ((xsd)(IDREFS)) \
         ((xsd)(int)) \
         ((xsd)(integer)) \
         ((xsd)(language)) \
         ((xsd)(long)) \
         ((xsd)(Name)) \
         ((xsd)(NCName)) \
         ((xsd)(negativeInteger)) \
         ((xsd)(NMTOKEN)) \
         ((xsd)(nonNegativeInteger)) \
         ((xsd)(nonPositiveInteger)) \
         ((xsd)(normalizedString)) \
         ((xsd)(NOTATION)) \
         ((xsd)(positiveInteger)) \
         ((xsd)(QName)) \
         ((xsd)(short)) \
         ((xsd)(string)) \
         ((xsd)(time)) \
         ((xsd)(token)) \
         ((xsd)(unsignedByte)) \
         ((xsd)(unsignedInt)) \
         ((xsd)(unsignedLong)) \
         ((xsd)(unsignedShort)) \
/* */
namespace owlcpp{ namespace terms{
OWLCPP_GENERATE_NODE_TAGS(OWLCPP_TERMS_XSD, OWLCPP_TERMS_XSD_N0)
}}

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

#endif /* NODE_TAGS_OWL_HPP_ */
