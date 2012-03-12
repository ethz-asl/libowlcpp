/** @file "/owlcpp/lib/reasoner/triple_to_fact_adaptor.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include "boost/assert.hpp"
#include "owlcpp/reasoner/detail/triple_to_fact_adaptor.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/*
*******************************************************************************/
void Triple_to_fact_adaptor::submit(Triple const& t) {
   //ignore blank nodes
//   if( ts_[t.subject()].ns_id() == N_blank::id() ) return;
   switch (t.predicate()()) {
      case T_rdf_type::index:
         submit_type_triple(t);
         return;
      case T_rdfs_subClassOf::index:
         k_.impliesConcepts(concept(t.subject()), concept(t.object()));
         return;
      case T_owl_equivalentClass::index:
         e_manager().newArgList();
         e_manager().addArg(concept(t.subject()));
         e_manager().addArg(concept(t.object()));
         k_.equalConcepts();
         return;
      case T_owl_disjointWith::index:
         e_manager().newArgList();
         e_manager().addArg(concept(t.subject()));
         e_manager().addArg(concept(t.object()));
         k_.disjointConcepts();
         return;
      case T_owl_AllDisjointClasses::index: {
         e_manager().newArgList();
         boost::iterator_range<Rdf_list_iter_s> r = rdf_list(t.subject(), ts_);
         BOOST_ASSERT(distance(r) > 2);
         BOOST_FOREACH(const Node_id nid, r) e_manager().addArg(concept(nid));
         k_.disjointConcepts();
         return;
      }
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
         submit_custom_triple(t);
   }
}

/*
*******************************************************************************/
void Triple_to_fact_adaptor::submit_custom_triple(Triple const& t) {
   const Node_type nt = node_type(t.predicate());
   if( nt.is_obj_property() ) {
      k_.relatedTo(
               instance(t.subject()),
               obj_role(t.predicate()),
               instance(t.object())
      );
   } else if(nt.is_data_property() ) {
      k_.valueOf(
               instance(t.subject()),
               data_role(t.predicate()),
               data_value(t.object())
      );
   }
   BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown predicate type")
            << Err::str1_t(ts_.string(t.predicate()))
      );
}
/*
*******************************************************************************/
void Triple_to_fact_adaptor::submit_type_triple(Triple const& t) {
   BOOST_ASSERT(t.pred_ == T_rdf_type::id());
   //ignore blank nodes
   if( ts_[t.subject()].ns_id() == N_blank::id() ) return;
   switch (t.object()()) {
      case T_owl_Class::index:
         concept(t.subject());
         return;
      case T_rdfs_Datatype::index:
         datatype(t.subject());
         return;
      case T_owl_ObjectProperty::index:
         obj_role(t.subject());
         return;
      case T_owl_AsymmetricProperty::index:
         k_.setAsymmetric( obj_role(t.subject()) );
         return;
      case T_owl_FunctionalProperty::index:
         k_.setOFunctional( obj_role(t.subject()) );
         return;
      case T_owl_InverseFunctionalProperty::index:
         k_.setInverseFunctional( obj_role(t.subject()) );
         return;
      case T_owl_inverseOf::index:
         k_.setInverseRoles( obj_role(t.subject()), obj_role(t.object()) );
         return;
      case T_owl_IrreflexiveProperty::index:
         k_.setIrreflexive( obj_role(t.subject()) );
         return;
      case T_owl_ReflexiveProperty::index:
         k_.setReflexive( obj_role(t.subject()) );
         return;
      case T_owl_SymmetricProperty::index:
         k_.setSymmetric( obj_role(t.subject()) );
         return;
      case T_owl_TransitiveProperty::index:
         k_.setTransitive( obj_role(t.subject()) );
         return;
      case T_rdfs_subPropertyOf::index:
         k_.impliesORoles( obj_role(t.subject()), obj_role(t.object()) );
         return;
      case T_owl_DatatypeProperty::index:
         data_role(t.subject());
         return;
      case T_owl_NamedIndividual::index:
         instance( t.subject() );
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
                  Err()
                  << Err::msg_t("unsupported rdf:type object")
                  << Err::str1_t(ts_.string(t.object()))
         );
   }
}

/*
*******************************************************************************/
TDLConceptExpression* Triple_to_fact_adaptor::concept_expression(const Node_id nid) {

}

/*
*******************************************************************************/
TDLConceptExpression* Triple_to_fact_adaptor::concept(const Node_id nid) {
   if( ts_[nid].ns_id() != N_blank::id() )
      return e_manager().Concept(ts_.string(nid));
   return concept_expression(nid);
}

/*
*******************************************************************************/
TDLIndividualExpression* Triple_to_fact_adaptor::instance(const Node_id nid) {
   if( ts_[nid].ns_id() != N_blank::id() )
      return e_manager().Individual(ts_.string(nid));
   //TODO:
   return 0;
}

/*
*******************************************************************************/
TDLIndividualExpression*
Triple_to_fact_adaptor::instance_of(const Node_id inst_id, const Node_id cls_id) {
   TDLIndividualExpression* inst = instance( inst_id );
   const TDLConceptExpression* cls = concept( cls_id );
   k_.instanceOf(inst, cls);
   return inst;
}

/*
*******************************************************************************/
TDLObjectRoleExpression*
Triple_to_fact_adaptor::obj_role_expression(const Node_id nid) {

}

/*
*******************************************************************************/
TDLObjectRoleExpression* Triple_to_fact_adaptor::obj_role(const Node_id nid) {
   if( ts_[nid].ns_id() != N_blank::id() )
      return e_manager().ObjectRole(ts_.string(nid));
   return obj_role_expression(nid);
}

/*
*******************************************************************************/
TDLDataRoleExpression* Triple_to_fact_adaptor::data_role(const Node_id nid) {
   if( ts_[nid].ns_id() != N_blank::id() )
      return e_manager().DataRole(ts_.string(nid));
   //TODO:
   return 0;
}

/*
*******************************************************************************/
TDLDataTypeExpression* Triple_to_fact_adaptor::datatype_expression(const Node_id nid) {
   //todo:
   return 0;
}

/*
*******************************************************************************/
TDLDataTypeExpression* Triple_to_fact_adaptor::datatype(const Node_id nid) {
   if( ts_[nid].ns_id() == N_blank::id() ) return datatype_expression(nid);
   switch (nid()) {
   case T_empty_::index:
   case T_xsd_string::index:
   case T_xsd_anyURI::index:
   case T_xsd_normalizedString::index:
   case T_xsd_token::index:
   case T_xsd_language::index:
   case T_xsd_NMTOKEN::index:
   case T_xsd_Name::index:
   case T_xsd_NCName::index:
      return e_manager().getStrDataType();
   case T_xsd_boolean::index:
      return e_manager().getBoolDataType();
   case T_xsd_decimal::index:
   case T_xsd_float::index:
   case T_xsd_double::index:
      return e_manager().getRealDataType();
   case T_xsd_dateTime::index:
   case T_xsd_time::index:
   case T_xsd_date::index:
   case T_xsd_gYearMonth::index:
   case T_xsd_gYear::index:
   case T_xsd_gMonthDay::index:
   case T_xsd_gDay::index:
   case T_xsd_gMonth::index:
      return e_manager().getTimeDataType();
   case T_xsd_hexBinary::index:
   case T_xsd_base64Binary::index:
   case T_xsd_integer::index:
   case T_xsd_nonPositiveInteger::index:
   case T_xsd_negativeInteger::index:
   case T_xsd_long::index:
   case T_xsd_int::index:
   case T_xsd_short::index:
   case T_xsd_byte::index:
   case T_xsd_nonNegativeInteger::index:
   case T_xsd_unsignedLong::index:
   case T_xsd_unsignedInt::index:
   case T_xsd_unsignedShort::index:
   case T_xsd_unsignedByte::index:
   case T_xsd_positiveInteger::index:
      return e_manager().getIntDataType();
   default:
      return e_manager().DataType(ts_.string(nid));
   }
}

/*
*******************************************************************************/
TDLDataValue const* Triple_to_fact_adaptor::data_value(const Node_id nid) {
   Node const& node = ts_[nid];
   if( node.ns_id() != N_empty::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("literal node is expected")
            << Err::str1_t(ts_.string(nid))
   );
   const Node_id dt = ts_.datatype(nid);
   return e_manager().DataValue(node.value_str(), datatype(dt));
}

/*
*******************************************************************************/
TExpressionManager& Triple_to_fact_adaptor::e_manager() {
   return *k_.getExpressionManager();
}

/*
*******************************************************************************/
Node_type Triple_to_fact_adaptor::node_type(const Node_id nid) const {
   Node_type nt;
   BOOST_FOREACH(
            Triple const& t,
            ts_.triples().find(nid, T_rdf_type::id(), any(), any())) {
      nt.set(t.object());
   }

   BOOST_FOREACH(
            Triple const& t,
            ts_.triples().find(any(), T_owl_annotatedSource::id(), nid, any())) {

      const Node_id x = t.subject();
      if( ts_[x].ns_id() != N_blank::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("non-blank owl:annotatedSource x")
               << Err::str1_t(ts_.string(nid))
      );
      BOOST_FOREACH(
               Triple const& t,
               ts_.triples().find(x, T_owl_annotatedTarget::id(), any(), any())) {
         nt.set(t.object());
      }
   }
   return nt;
}

/*
*******************************************************************************/
}//namespace detail
}//namespace owlcpp
