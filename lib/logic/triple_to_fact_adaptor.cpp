/** @file "/owlcpp/lib/logic/triple_to_fact_adaptor.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_REASONER_SOURCE
#define OWLCPP_REASONER_SOURCE
#endif
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"

#include <vector>
#include "boost/assert.hpp"

#include "owlcpp/rdf/query_triples.hpp"
#include "factpp/Kernel.hpp"
#include "expressions.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::axiom(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   switch (pred()) {
   case T_rdf_type::index:
      return axiom_rdf_type(t);
   case T_rdfs_subClassOf::index:
      return k_.impliesConcepts(concept(subj), concept(obj));
   case T_owl_equivalentClass::index:
      return equivalent_class(t);
      e_m().newArgList();
      e_m().addArg(concept(subj));
      e_m().addArg(concept(obj));
      return k_.equalConcepts();
   case T_owl_disjointWith::index:
      e_m().newArgList();
      e_m().addArg(concept(subj));
      e_m().addArg(concept(obj));
      return k_.disjointConcepts();
   case T_owl_disjointUnionOf::index:
      return disjoint_union(t);
   case T_owl_inverseOf::index:
      if( ! declaration<Node_property>(subj).is_object() )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("x in \"x owl:inverseOf y\" is not declared as object property")
      );
      if( ! declaration<Node_property>(obj).is_object() )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("y in \"x owl:inverseOf y\" is not declared as object property")
      );
      return k_.setInverseRoles(obj_property(subj), obj_property(obj));
   case T_rdfs_subPropertyOf::index:
      return sub_property_of(t);
   case T_owl_propertyChainAxiom::index:
      return property_chain(t);
   case T_owl_equivalentProperty::index:
      return equivalent_property(t);
   case T_owl_propertyDisjointWith::index:
      return property_disjoint_with(t);
   case T_rdfs_domain::index:
      return domain(t);
   case T_rdfs_range::index:
      return range(t);
   case T_owl_hasKey::index:
      BOOST_THROW_EXCEPTION( Err() << Err::msg_t("owl:hasKey is not supported") );
   case T_owl_sameAs::index:
      e_m().newArgList();
      e_m().addArg(instance(subj));
      e_m().addArg(instance(obj));
      return k_.processSame();
   case T_owl_differentFrom::index:
      e_m().newArgList();
      e_m().addArg(instance(subj));
      e_m().addArg(instance(obj));
      return k_.processDifferent();
/*
   {
      if( ts_[subj].ns_id() == N_blank::id() ) return 0;
      const Node_property nts = declaration<Node_property>(subj);
      const Node_property nto = declaration<Node_property>(obj);
      if( nts != nto ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("declaration mismatch in rdfs:subPropertyOf triple")
               << Err::str1_t(ts_.string(subj))
               << Err::str2_t(ts_.string(obj))
      );
      if( nts.is_object() ) {
         k_.impliesORoles(obj_property(subj), obj_property(obj));
      } else if( nts.is_data() ) {
         k_.impliesDRoles(data_property(subj), data_property(obj));
      }
      return 0;
   }
*/
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
      return 0;
   default:
      return axiom_custom_predicate(t);
   }
}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::axiom_rdf_type(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   switch (obj()) {
   case T_owl_AllDisjointClasses::index:
   case T_owl_AllDisjointProperties::index:
      return all_disjoint(t);
   case T_owl_AllDifferent::index:
      return all_different(t);
   case T_owl_FunctionalProperty::index: {
      const Node_property np = declaration<Node_property>(subj);
      if( np.is_object() ) return k_.setOFunctional(obj_property(subj));
      if( np.is_data() ) return k_.setDFunctional(data_property(subj));
      BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("undefined subject in x rdf:type owl:FunctionalProperty")
                  << Err::str1_t(ts_.string(subj))
      );
   }

   case T_owl_InverseFunctionalProperty::index:
      return k_.setInverseFunctional(obj_property(subj));

   case T_owl_ReflexiveProperty::index:
      if( ! declaration<Node_property>(subj).is_object() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("undefined subject in x rdf:type owl:ReflexiveProperty")
               << Err::str1_t(ts_.string(subj))
      );
      return k_.setReflexive(obj_property(subj));

   case T_owl_IrreflexiveProperty::index:
      if( ! declaration<Node_property>(subj).is_object() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("undefined subject in x rdf:type owl:owl:IrreflexiveProperty")
               << Err::str1_t(ts_.string(subj))
      );
      return k_.setIrreflexive(obj_property(subj));

   case T_owl_SymmetricProperty::index:
      return k_.setSymmetric(obj_property(subj));

   case T_owl_AsymmetricProperty::index:
      if( ! declaration<Node_property>(subj).is_object() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("undefined subject in x rdf:type owl:owl:AsymmetricProperty")
               << Err::str1_t(ts_.string(subj))
      );
      return k_.setAsymmetric(obj_property(subj));

   case T_owl_TransitiveProperty::index:
      return k_.setTransitive(obj_property(subj));

   default:
      if( declaration<Node_type>(obj).is_class() )
         return k_.instanceOf(instance(subj), concept(obj));
      break;
   }
   BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unsupported rdf:type object")
                  << Err::str1_t(ts_.string(subj))
   );
}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::all_disjoint(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id obj = t.object();
   Node const& snode = ts_[subj];
   if( ! is_blank(snode.ns_id()) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("blank subject expected in _:x rdf:type owl:AllDisjoint*")
            << Err::str1_t(ts_.string(subj))
   );
   Query<1,1,0,0>::range r = ts_.triples().find(subj, T_owl_members::id(), any(), any());
   if( ! r ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("_:x owl:members seq triple not found for _:x rdf:type owl:AllDisjoint*")
            << Err::str1_t(ts_.string(subj))
   );
   const Node_id seqid = r.front().object();
   const std::vector<Node_id> seq(
            boost::copy_range<std::vector<Node_id> >(rdf_list(seqid, ts_))
   );
   if( seq.size() < 2 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("less than 2 members for owl:AllDisjoint*")
            << Err::str1_t(ts_.string(seqid))
   );

   if( obj == T_owl_AllDisjointClasses::id() ) {
      BOOST_FOREACH(const Node_id nid, seq) {
         if( ! declaration<Node_type>(nid).is_class() ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("non-class node in owl:AllDisjointClasses")
                  << Err::str1_t(ts_.string(nid))
         );
      }
      e_m().newArgList();
      BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(concept(nid));
      return k_.disjointConcepts();
   }

   if( obj != T_owl_AllDisjointProperties::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported owl:AllDisjoint* axiom")
            << Err::str1_t(ts_.string(obj))
   );

   const Node_property np = declaration<Node_property>(seq[0]);
   if( ! np.is_object() || ! np.is_data() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("object or data property types are expected in owl:AllDisjointProperties axiom")
            << Err::str1_t(ts_.string(seq[0]))
   );
   BOOST_FOREACH(const Node_id nid, seq) {
      if( np != declaration<Node_property>(nid) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("non-uniform types in owl:AllDisjointProperties axiom")
               << Err::str1_t(ts_.string(nid))
      );
   }

   e_m().newArgList();
   if( np.is_object() ) {
      BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(obj_property(nid));
      return k_.disjointORoles();
   }

   BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(data_property(nid));
   return k_.disjointDRoles();
}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::axiom_custom_predicate(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::disjoint_union(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::sub_property_of(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::equivalent_property(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::equivalent_class(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::property_chain(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::property_disjoint_with(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::domain(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::range(Triple const& t) {

}

/*
*******************************************************************************/
TDLAxiom* Triple_to_fact_adaptor::all_different(Triple const& t) {

}

/*
void Triple_to_fact_adaptor::submit(Triple const& t) {
   //ignore blank nodes
//   if( ts_[t.subject()].ns_id() == N_blank::id() ) return;
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   switch (pred()) {
   case T_rdfs_subClassOf::index:
      k_.impliesConcepts(concept(subj), concept(obj));
      return;
   case T_owl_equivalentClass::index:
      e_m().newArgList();
      e_m().addArg(concept(subj));
      e_m().addArg(concept(obj));
      k_.equalConcepts();
      return;
   case T_owl_disjointWith::index:
      e_m().newArgList();
      e_m().addArg(concept(subj));
      e_m().addArg(concept(obj));
      k_.disjointConcepts();
      return;
   case T_owl_inverseOf::index:
      if( ts_[subj].ns_id() == N_blank::id() ) return;
      k_.setInverseRoles(obj_property(subj), obj_property(obj));
      return;
   case T_rdfs_subPropertyOf::index: {
      if( ts_[subj].ns_id() == N_blank::id() ) return;
      const Node_property nts = declaration<Node_property>(subj);
      const Node_property nto = declaration<Node_property>(obj);
      if( nts != nto ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("declaration mismatch in rdfs:subPropertyOf triple")
      << Err::str1_t(ts_.string(subj))
      << Err::str2_t(ts_.string(obj))
      );
      if( nts.is_object() ) {
         k_.impliesORoles(obj_property(subj), obj_property(obj));
      } else if( nts.is_data() ) {
         k_.impliesDRoles(data_property(subj), data_property(obj));
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
*******************************************************************************/

/*
*******************************************************************************/
void Triple_to_fact_adaptor::submit_custom_triple(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   const Node_property np = declaration<Node_property>(pred);
   if( np.is_object() ) {
      k_.relatedTo(
               instance(subj),
               obj_property(pred),
               instance(obj)
      );
      return;
   }

   if(np.is_data() ) {
      k_.valueOf(
               instance(subj),
               data_property(pred),
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
TDLConceptExpression* Triple_to_fact_adaptor::concept(const Node_id nid) {
   return make_fact_expression<Obj_class>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLIndividualExpression* Triple_to_fact_adaptor::instance(const Node_id nid) {
   Node const& node = ts_[nid];
   if( is_blank(node.ns_id()) || is_empty(node.ns_id()) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("invalid node for object instance declaration")
            << Err::str1_t(ts_.string(nid))
   );
   return e_m().Individual(ts_.string(nid));
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
TDLObjectRoleExpression* Triple_to_fact_adaptor::obj_property(const Node_id nid) {
   return make_fact_expression<Obj_prop>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLDataRoleExpression* Triple_to_fact_adaptor::data_property(const Node_id nid) {
   return make_fact_expression<Data_prop>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLDataTypeExpression* Triple_to_fact_adaptor::datatype(const Node_id nid) {
   return make_fact_expression<Data_class>( nid, ts_ )->get(k_);
/*
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
      return e_m().DataType(ts_.string(nid));
   }
*/
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
   return e_m().DataValue(node.value_str(), datatype(dt));
}

/*
*******************************************************************************/
TExpressionManager& Triple_to_fact_adaptor::e_m() {
   return *k_.getExpressionManager();
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
   const Node_property nt = declaration<Node_property>(prop);
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
      return k_.relatedToNot(instance(src_ind), obj_property(prop), instance(target));
   } else {
      return k_.valueOfNot(instance(src_ind), data_property(prop), data_value(target));
   }
}

/*
*******************************************************************************/
}//namespace detail
}//namespace owlcpp
