/** @file "/owlcpp/lib/logic/triple_to_fact_adaptor.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_REASONER_SOURCE
#define OWLCPP_REASONER_SOURCE
#endif
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"

#include "boost/assert.hpp"

#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "factpp/Kernel.hpp"
#include "expressions.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/*
*******************************************************************************/
void Triple_to_fact_adaptor::submit(Triple const& t) {
   //ignore blank nodes
//   if( ts_[t.subject()].ns_id() == N_blank::id() ) return;
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   switch (pred()) {
      case T_rdf_type::index:
         submit_type_triple(t);
         return;
      case T_rdfs_subClassOf::index:
         k_.impliesConcepts(concept(subj), concept(obj));
         return;
      case T_owl_equivalentClass::index:
         e_manager().newArgList();
         e_manager().addArg(concept(subj));
         e_manager().addArg(concept(obj));
         k_.equalConcepts();
         return;
      case T_owl_disjointWith::index:
         e_manager().newArgList();
         e_manager().addArg(concept(subj));
         e_manager().addArg(concept(obj));
         k_.disjointConcepts();
         return;
      case T_owl_inverseOf::index:
         if( ts_[subj].ns_id() == N_blank::id() ) return;
         k_.setInverseRoles(obj_role(subj), obj_role(obj));
         return;
      case T_rdfs_subPropertyOf::index: {
         if( ts_[subj].ns_id() == N_blank::id() ) return;
         const Node_property nts = declaration(subj).second;
         const Node_property nto = declaration(obj).second;
         if( nts != nto ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("declaration mismatch in rdfs:subPropertyOf triple")
                  << Err::str1_t(ts_.string(subj))
                  << Err::str2_t(ts_.string(obj))
            );
         if( nts.is_object() ) {
            k_.impliesORoles(obj_role(subj), obj_role(obj));
         } else if( nts.is_data() ) {
            k_.impliesDRoles(data_role(subj), data_role(obj));
         }
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
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   const Node_property np = declaration(pred).second;
   if( np.is_object() ) {
      k_.relatedTo(
               instance(subj),
               obj_role(pred),
               instance(obj)
      );
      return;
   }

   if(np.is_data() ) {
      k_.valueOf(
               instance(subj),
               data_role(pred),
               data_value(obj)
      );
      return;
   }

   if( np.is_annotation() ) return;
   if( ts_[subj].ns_id() == N_blank::id() ) return;

   BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown predicate type")
            << Err::str1_t(ts_.string(pred))
      );
}
/*
*******************************************************************************/
void Triple_to_fact_adaptor::submit_type_triple(Triple const& t) {
   BOOST_ASSERT(t.pred_ == T_rdf_type::id());
   const Node_id subj = t.subject();
   const Node_id obj = t.object();
   switch (obj()) {
   case T_owl_NegativePropertyAssertion::index:
      negative_property_assertion(subj);
      return;
   }

   //ignore blank nodes
   if( ts_[subj].ns_id() == N_blank::id() ) return;
   switch (obj()) {
      case T_owl_Class::index:
         concept(subj);
         return;
      case T_owl_AllDisjointClasses::index: {
         e_manager().newArgList();
         boost::iterator_range<Rdf_list_iter_s> r = rdf_list(subj, ts_);
         BOOST_ASSERT(distance(r) > 2);
         BOOST_FOREACH(const Node_id nid, r) e_manager().addArg(concept(nid));
         k_.disjointConcepts();
         return;
      }
      case T_rdfs_Datatype::index:
         datatype(subj);
         return;
      case T_owl_ObjectProperty::index:
         obj_role(subj);
         return;
      case T_owl_AsymmetricProperty::index:
         k_.setAsymmetric( obj_role(subj) );
         return;
      case T_owl_FunctionalProperty::index:
         k_.setOFunctional( obj_role(subj) );
         return;
      case T_owl_InverseFunctionalProperty::index:
         k_.setInverseFunctional( obj_role(subj) );
         return;
      case T_owl_inverseOf::index:
         k_.setInverseRoles( obj_role(subj), obj_role(obj) );
         return;
      case T_owl_IrreflexiveProperty::index:
         k_.setIrreflexive( obj_role(subj) );
         return;
      case T_owl_ReflexiveProperty::index:
         k_.setReflexive( obj_role(subj) );
         return;
      case T_owl_SymmetricProperty::index:
         k_.setSymmetric( obj_role(subj) );
         return;
      case T_owl_TransitiveProperty::index:
         k_.setTransitive( obj_role(subj) );
         return;
      case T_rdfs_subPropertyOf::index:
         k_.impliesORoles( obj_role(subj), obj_role(obj) );
         return;
      case T_owl_DatatypeProperty::index:
         data_role(subj);
         return;
      case T_owl_NamedIndividual::index:
         instance( subj );
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
                  << Err::str1_t(ts_.string(obj))
         );
   }
}

/*
*******************************************************************************/
TDLConceptExpression* Triple_to_fact_adaptor::concept(const Node_id nid) {
   return Obj_type::make( nid, ts_ )->get(ts_, k_);
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
Triple_to_fact_adaptor::declaration_t
Triple_to_fact_adaptor::declaration(const Node_id nid) const {
   declaration_t d;
   BOOST_FOREACH(
            Triple const& t,
            ts_.triples().find(nid, T_rdf_type::id(), any(), any())) {
      d.first.set(t.object());
      d.second.set(t.object());
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
         d.first.set(t.object());
         d.second.set(t.object());
      }
   }
   return d;
}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::negative_property_assertion(const Node_id nid) {
   Query<1,1,0,0>::range r1 =
            ts_.triples().find(nid, T_owl_sourceIndividual::id(), any(), any());
   if( ! r1 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no owl:sourceIndividual in owl:NegativePropertyAssertion")
            << Err::str1_t(ts_.string(nid))
   );
   const Node_id src_ind = r1.front().object();

   Query<1,1,0,0>::range r2 =
            ts_.triples().find(nid, T_owl_assertionProperty::id(), any(), any());
   if( ! r2 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no owl:assertionProperty in owl:NegativePropertyAssertion")
            << Err::str1_t(ts_.string(nid))
            << Err::str2_t(ts_.string(src_ind))
   );
   const Node_id prop = r2.front().object();
   const Node_property nt = declaration(prop).second;
   if( ! nt.is_object() && ! nt.is_data() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("undefined property in owl:NegativePropertyAssertion")
            << Err::str1_t(ts_.string(prop))
   );
   const Node_id tiv = nt.is_object() ?
            T_owl_targetIndividual::id() :
            T_owl_targetValue::id();
   Query<1,1,0,0>::range r3 =
            ts_.triples().find(nid, tiv, any(), any());
   if( ! r3 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no owl:target* in owl:NegativePropertyAssertion")
            << Err::str1_t(ts_.string(nid))
            << Err::str2_t(ts_.string(src_ind))
            << Err::str3_t(ts_.string(prop))
   );
   const Node_id target = r3.front().object();

   if( nt.is_object() ) {
      /*return*/ k_.relatedToNot(instance(src_ind), obj_role(prop), instance(target));
   } else {
      /*return*/ k_.valueOfNot(instance(src_ind), data_role(prop), data_value(target));
   }
}

/*
*******************************************************************************/
}//namespace detail
}//namespace owlcpp
