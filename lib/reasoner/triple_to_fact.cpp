/** @file "/owlcpp/lib/reasoner/triple_to_fact.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_REASONER_SOURCE
#define OWLCPP_REASONER_SOURCE
#endif
#include "boost/assert.hpp"
#include "owlcpp/reasoner/triple_to_fact.hpp"
#include "owlcpp/reasoner/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace detail{

/*
*******************************************************************************/
void submit_type_triple(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool lax
) {
   using namespace owlcpp::terms;
   BOOST_ASSERT(t.pred_ == T_rdf_type::id());
   switch (t.object()()) {
      case T_owl_Class::index:
         //TODO
         return;
      case T_rdfs_Datatype::index:
         //TODO
         return;
      case T_owl_ObjectProperty::index:
         //TODO
         return;
      case T_owl_DatatypeProperty::index:
         //TODO
         return;
      case T_owl_NamedIndividual::index:
         //TODO
         return;
      //ignore, look for owl:annotatedTarget
      case T_owl_AnnotationProperty::index:
      case T_owl_Axiom::index:
      //ignore, look for rdf:type (owl:Class rdfs:Datatype owl:DataRange owl:Restriction)
      case T_rdfs_Class::index:
      //ignore, look for rdf:type (owl:***Property )
      case T_rdf_Property::index:
      //ignore, look for rdf:first, rdf:rest
      case T_rdf_List::index:
      //just ignore
      case T_owl_Ontology::index:
      case T_owl_OntologyProperty::index:
         return;
      default:
         BOOST_THROW_EXCEPTION(
                  Reasoner_err()
                  << Reasoner_err::msg_t("unsupported rdf:type object")
                  << Reasoner_err::str1_t(ts.string(t.object()))
         );
   }
}

/*
*******************************************************************************/
void submit_triple(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool lax
) {
   using namespace owlcpp::terms;
   switch (t.predicate()()) {
      case T_rdf_type::index:
         submit_type_triple(t, ts, kernel, lax);
         return;
      //ignored triples:
      case T_owl_annotatedProperty::index:
      case T_owl_annotatedSource::index:
      case T_owl_annotatedTarget::index:
      case T_owl_assertionProperty::index:
      case T_owl_distinctMembers::index:
      case T_owl_imports::index:
      case T_owl_members::index:
      case T_owl_onClass::index:
      case T_owl_onProperty::index:
      case T_owl_sourceIndividual::index:
      case T_owl_targetIndividual::index:
      case T_owl_versionInfo::index:
      case T_owl_versionIRI::index:
      case T_rdf_first::index:
      case T_rdf_rest::index:
      case T_rdfs_comment::index:
      case T_rdfs_isDefinedBy::index:
      case T_rdfs_label::index:
         return;
      default:
         BOOST_THROW_EXCEPTION(
                  Reasoner_err()
                  << Reasoner_err::msg_t("unsupported predicate")
                  << Reasoner_err::str1_t(ts.string(t.predicate()))
         );
   }
}

}//namespace detail
}//namespace owlcpp
