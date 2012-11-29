/** @file "/owlcpp/lib/logic/factpp/adaptor_triple.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_LOGIC_SOURCE
#define OWLCPP_LOGIC_SOURCE
#endif
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"

#include <vector>
#include "boost/assert.hpp"

#include "owlcpp/rdf/query_triples.hpp"
#include "factpp/Kernel.hpp"
#include "expression.hpp"
#include "obj_type.hpp"
#include "obj_property.hpp"
#include "logic/node_property_declaration.hpp"
#include "logic/node_type_declaration.hpp"
#include "logic/find_node_declaration.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/rdf/query_node.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
void Adaptor_triple::submit(Triple const& t) {
   try{
      axiom(t);
      //k_.isKBConsistent(); //check if triple crashes reasoner
   } catch(...) {
      BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("error submitting triple")
      << Err::str1_t(
            to_string(t.subject(), ts_) + ' ' +
            to_string(t.predicate(), ts_) + ' ' +
            to_string(t.object(), ts_)
      )
      << Err::nested_t(boost::current_exception())
      );
   }
}


/*
*******************************************************************************/
TDLAxiom* Adaptor_triple::axiom(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   switch (pred()) {
   case T_rdf_type::index:
      return axiom_type(t);

   case T_rdfs_subClassOf::index:
      check_declaration(subj, Node_type::object(), ts_);
      check_declaration(obj, Node_type::object(), ts_);
      return k_.impliesConcepts(obj_type(subj), obj_type(obj));

   case T_owl_equivalentClass::index: {
      const Node_type nt = check_same_declaration<Node_type>(subj, obj, ts_);
      if( nt.is_object() ) {
         e_m().newArgList();
         e_m().addArg(obj_type(subj));
         e_m().addArg(obj_type(obj));
         return k_.equalConcepts();
      }
//      if( ! is_iri(ts_[subj].ns_id()) ) return 0;
      BOOST_THROW_EXCEPTION( Err() << Err::msg_t("owl:equivalentClass on datatypes is not supported") );
   }

   case T_owl_disjointWith::index:
      check_declaration(subj, Node_type::object(), ts_);
      check_declaration(obj, Node_type::object(), ts_);
      e_m().newArgList();
      e_m().addArg(obj_type(subj));
      e_m().addArg(obj_type(obj));
      return k_.disjointConcepts();

   case T_owl_disjointUnionOf::index:
      if( ! is_iri(ts_[subj].ns_id()) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("non-IRI subject in *:x owl:disjointUnionOf seq")
               << Err::str1_t(to_string(subj, ts_))
      );
      return axiom_from_seq(pred, obj, 2, subj);

   case T_owl_inverseOf::index:
      if( ! is_iri(ts_[subj].ns_id()) ) return 0;
      check_declaration(obj, Node_property::object(), ts_);
      return k_.setInverseRoles(obj_property(subj), obj_property(obj));

   case T_rdfs_subPropertyOf::index: {
      const Node_property np =
            check_same_declaration<Node_property>(subj, obj, ts_);
      if( np.is_object() ) {
         return k_.impliesORoles(obj_property(subj), obj_property(obj));
      }
      if( np.is_data() ) {
         return k_.impliesDRoles(data_property(subj), data_property(obj));
      }
      return 0;
   }

   case T_owl_propertyChainAxiom::index:
      return axiom_from_seq(pred, obj, 2, subj);

   case T_owl_equivalentProperty::index: {
      const Node_property np = check_same_declaration<Node_property>(subj, obj, ts_);
      if( np.is_object() ) {
         e_m().newArgList();
         e_m().addArg(obj_property(subj));
         e_m().addArg(obj_property(obj));
         return k_.equalORoles();
      }
      if( np.is_data() ) {
         e_m().newArgList();
         e_m().addArg(data_property(subj));
         e_m().addArg(data_property(obj));
         return k_.equalDRoles();
      }
   }

   case T_owl_propertyDisjointWith::index:{
      const Node_property np = check_same_declaration<Node_property>(subj, obj, ts_);
      if( np.is_object() ) {
         e_m().newArgList();
         e_m().addArg(obj_property(subj));
         e_m().addArg(obj_property(obj));
         return k_.disjointORoles();
      }
      if( np.is_data() ) {
         e_m().newArgList();
         e_m().addArg(data_property(subj));
         e_m().addArg(data_property(obj));
         return k_.disjointDRoles();
      }
   }

   case T_rdfs_domain::index: {
      check_declaration(obj, Node_type::object(), ts_);
      const Node_property np = declaration<Node_property>(subj, ts_);
      if( np.is_object() ) return k_.setODomain(obj_property(subj), obj_type(obj));
      if( np.is_data() ) return k_.setDDomain(data_property(subj), obj_type(obj));
   }

   case T_rdfs_range::index:{
      const Node_property np = declaration<Node_property>(subj, ts_);
      if( np.is_object() ) {
         check_declaration(obj, Node_type::object(), ts_);
         return k_.setORange(obj_property(subj), obj_type(obj));
      }
      if( np.is_data() ) {
         check_declaration(obj, Node_type::data(), ts_);
         return k_.setDRange(data_property(subj), data_type(obj));
      }
   }

   case T_owl_hasKey::index:
      BOOST_THROW_EXCEPTION( Err() << Err::msg_t("owl:hasKey is not supported") );

   case T_owl_sameAs::index:
      e_m().newArgList();
      e_m().addArg(obj_value(subj));
      e_m().addArg(obj_value(obj));
      return k_.processSame();

   case T_owl_differentFrom::index:
      e_m().newArgList();
      e_m().addArg(obj_value(subj));
      e_m().addArg(obj_value(obj));
      return k_.processDifferent();

   case T_owl_oneOf::index:
      if( is_iri(ts_[subj].ns_id()) ) return axiom_from_seq(pred, obj, 0, subj);
      return 0;

   case T_owl_complementOf::index:
      if( is_iri(ts_[subj].ns_id()) ) {
         check_declaration(subj, Node_type::object(), ts_);
         check_declaration(obj, Node_type::object(), ts_);
         e_m().newArgList();
         e_m().addArg(obj_type(subj));
         e_m().addArg(Ot_complement(obj, ts_).get(k_));
         return k_.equalConcepts();
      }

   case T_owl_intersectionOf::index:
   case T_owl_unionOf::index:
      if( is_iri(ts_[subj].ns_id()) ) {
         check_declaration(subj, Node_type::object(), ts_);
         e_m().newArgList();
         e_m().addArg(obj_type(subj));
         e_m().addArg(Ot_type_list(pred, obj, ts_).get(k_));
         return k_.equalConcepts();
      }
      return 0;

   //ignored triples:
   case T_owl_onProperty::index:
   case T_owl_datatypeComplementOf::index:
   case T_owl_onDatatype::index: //TODO
   case T_owl_withRestrictions::index: //TODO
   case T_owl_allValuesFrom::index:
   case T_owl_hasValue::index:
   case T_owl_hasSelf::index:
   case T_owl_minQualifiedCardinality::index:
   case T_owl_onClass::index:
   case T_owl_maxQualifiedCardinality::index:
   case T_owl_qualifiedCardinality::index:
   case T_owl_minCardinality::index:
   case T_owl_maxCardinality::index:
   case T_owl_cardinality::index:
   case T_owl_onProperties::index: //TODO
   case T_owl_onDataRange::index: //TODO
   case T_owl_someValuesFrom::index: //class expression, not axiom
      if( ! is_blank(ts_[subj].ns_id()) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("blank node subject is expected")
               << Err::str1_t(to_string(subj, ts_))
      );
      return 0;

   case T_owl_annotatedProperty::index:
   case T_owl_annotatedSource::index:
   case T_owl_annotatedTarget::index:
   case T_owl_assertionProperty::index:
   case T_owl_distinctMembers::index:
   case T_owl_imports::index:
   case T_owl_members::index:
   case T_owl_sourceIndividual::index:
   case T_owl_priorVersion::index:
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
TDLAxiom* Adaptor_triple::axiom_type(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id obj = t.object();
   Node const& subj_node = ts_[subj];
   switch (obj()) {

   case T_owl_FunctionalProperty::index: {
      const Node_property np = declaration<Node_property>(subj, ts_);
      if( np.is_object() ) return k_.setOFunctional(obj_property(subj));
      if( np.is_data() ) return k_.setDFunctional(data_property(subj));
   }

   case T_owl_InverseFunctionalProperty::index:
      check_declaration(subj, Node_property::object(), ts_);
      return k_.setInverseFunctional(obj_property(subj));

   case T_owl_ReflexiveProperty::index:
      check_declaration(subj, Node_property::object(), ts_);
      return k_.setReflexive(obj_property(subj));

   case T_owl_IrreflexiveProperty::index:
      check_declaration(subj, Node_property::object(), ts_);
      return k_.setIrreflexive(obj_property(subj));

   case T_owl_SymmetricProperty::index:
      check_declaration(subj, Node_property::object(), ts_);
      return k_.setSymmetric(obj_property(subj));

   case T_owl_AsymmetricProperty::index:
      check_declaration(subj, Node_property::object(), ts_);
      return k_.setAsymmetric(obj_property(subj));

   case T_owl_TransitiveProperty::index:
      check_declaration(subj, Node_property::object(), ts_);
      return k_.setTransitive(obj_property(subj));

   case T_owl_Class::index:
   case T_rdfs_Datatype::index:
   case T_owl_ObjectProperty::index:
   case T_owl_DatatypeProperty::index:
      if( is_iri(subj_node.ns_id()) ) return axiom_iri_node_type(t);
      return 0;

   case T_owl_NamedIndividual::index:
   case T_owl_DeprecatedClass::index:
   case T_owl_DeprecatedProperty::index:
      if( is_iri(subj_node.ns_id()) ) return axiom_iri_node_type(t);
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("IRI subject node is expected")
               << Err::str1_t(to_string(subj, ts_))
      );

   case T_owl_AllDisjointClasses::index:
   case T_owl_AllDisjointProperties::index:
   case T_owl_AllDifferent::index:
   case T_owl_NegativePropertyAssertion::index:
   case T_owl_Restriction::index:
      if( is_blank(subj_node.ns_id()) ) return axiom_blank_node_type(t);
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("blank subject node is expected")
               << Err::str1_t(to_string(subj, ts_))
      );

   //ignored triples
   case T_owl_Ontology::index:
   case T_owl_OntologyProperty::index:
   case T_owl_Axiom::index:
   case T_owl_AnnotationProperty::index:
   case T_rdfs_Class::index:
      return 0; //do not form axioms

   default:
      if( declaration<Node_type>(obj, ts_).is_object() )
         return k_.instanceOf(obj_value(subj), obj_type(obj));
      break;
   }
   BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unsupported rdf:type object")
                  << Err::str1_t(to_string(subj, ts_))
   );
}

/*
*******************************************************************************/
TDLAxiom* Adaptor_triple::axiom_iri_node_type(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id obj = t.object();
   BOOST_ASSERT( is_iri(ts_[subj].ns_id()) );

   switch (obj()) {

   case T_owl_Class::index:
      if( subj == T_owl_Thing::id() || subj == T_owl_Nothing::id() ) return 0;
      if( ts_.is_standard(subj) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("re-definition of a standard term")
               << Err::str1_t(to_string(subj, ts_))
      );
      return k_.declare( e_m().Concept(to_string(subj, ts_)) );

   case T_rdfs_Datatype::index:
      if( subj == T_rdfs_Literal::id() || subj == T_owl_Nothing::id() ) return 0;
      if( ts_.is_standard(subj) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("re-definition of a standard term")
               << Err::str1_t(to_string(subj, ts_))
      );
      return k_.declare( e_m().DataType(to_string(subj, ts_)) );

   case T_owl_ObjectProperty::index:
      return k_.declare( e_m().ObjectRole(to_string(subj, ts_)) );

   case T_owl_DatatypeProperty::index:
      return k_.declare( e_m().DataRole(to_string(subj, ts_)) );

   case T_owl_NamedIndividual::index:
      return k_.declare( e_m().Individual(to_string(subj, ts_)) );

   //ignored triples
   case T_owl_DeprecatedClass::index:
   case T_owl_DeprecatedProperty::index:
      return 0;

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported rdf:type object")
            << Err::str1_t(to_string(subj, ts_))
   );
   }
}

/*
*******************************************************************************/
TDLAxiom* Adaptor_triple::axiom_blank_node_type(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id obj = t.object();
   BOOST_ASSERT( is_blank(ts_[subj].ns_id()) );

   switch (obj()) {

   case T_owl_AllDisjointClasses::index:
   case T_owl_AllDisjointProperties::index: {
      Triple_store::result_b<1,1,0,0>::type r =
               ts_.find_triple(subj, T_owl_members::id(), any(), any());
      if( ! r ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t(
                        "_:x owl:members seq triple not found "
                        "for _:x rdf:type owl:AllDisjoint*"
               )
               << Err::str1_t(to_string(subj, ts_))
      );
      return axiom_from_seq(obj, r.front().object(), 2);
   }

   case T_owl_AllDifferent::index: {
      Triple_store::result_b<1,1,0,0>::type r =
               ts_.find_triple(subj, T_owl_members::id(), any(), any());
      if( ! r ) {
         r = ts_.find_triple(subj, T_owl_distinctMembers::id(), any(), any());
      }
      if( ! r ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t(
                        "_:x owl:(distinct)Members seq not found for "
                        "_:x rdf:type owl:AllDifferent"
               )
               << Err::str1_t(to_string(subj, ts_))
      );
      return axiom_from_seq(obj, r.front().object(), 2);
   }

   case T_owl_NegativePropertyAssertion::index:
      return negative_property_assertion(subj);

   case T_owl_Restriction::index: //class expression, not axiom
      return 0;

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported rdf:type object")
            << Err::str1_t(to_string(subj, ts_))
   );

   }
}

/*
*******************************************************************************/
TDLAxiom* Adaptor_triple::axiom_from_seq(
         const Node_id op,
         const Node_id seq_nid,
         const std::size_t min_len,
         const Node_id subj
) {
   const std::vector<Node_id> seq(
            boost::copy_range<std::vector<Node_id> >(rdf_list(seq_nid, ts_))
   );
   if( seq.size() < min_len ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("not enough elements for operation")
            << Err::str1_t(to_string(op, ts_))
            << Err::str2_t(to_string(seq_nid, ts_))
   );
   switch (op()) {
   case T_owl_disjointUnionOf::index:
      check_declaration(subj, Node_type::object(), ts_);
      check_seq_declaration(seq, Node_type::object(), ts_);
      e_m().newArgList();
      BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(obj_type(nid));
      return k_.disjointUnion(obj_type(subj));

   case T_owl_AllDisjointClasses::index:
      check_seq_declaration(seq, Node_type::object(), ts_);
      e_m().newArgList();
      BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(obj_type(nid));
      return k_.disjointConcepts();

   case T_owl_AllDisjointProperties::index: {
      const Node_property np = check_seq_declaration<Node_property>(seq, ts_);
      if( np.is_object() ) {
         e_m().newArgList();
         BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(obj_type(nid));
         return k_.disjointORoles();
      }
      if( np.is_data() ) {
         e_m().newArgList();
         BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(data_type(nid));
         return k_.disjointDRoles();
      }
      return 0;
   }

   case T_owl_propertyChainAxiom::index:
      check_declaration(subj, Node_property::object(), ts_);
      check_seq_declaration<Node_property>(seq, Node_property::object(), ts_);
      e_m().newArgList();
      BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(obj_property(nid));
      return k_.impliesORoles(e_m().Compose(), obj_property(subj));

   case T_owl_AllDifferent::index:
      e_m().newArgList();
      BOOST_FOREACH(const Node_id nid, seq) e_m().addArg(obj_value(nid));
      return k_.processDifferent();

   case T_owl_oneOf::index:
      check_declaration(subj, Node_type::object(), ts_);
      BOOST_FOREACH(const Node_id nid, seq) {
         if( ! is_iri(ts_[nid].ns_id()) ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("non-IRI node in owl:oneOf sequence")
                  << Err::str1_t(to_string(nid, ts_))
         );
      }
      e_m().newArgList();
      e_m().addArg(obj_type(subj));
      if( seq.empty() ) e_m().addArg(e_m().Bottom());
      else {
         e_m().newArgList();
         BOOST_FOREACH(const Node_id nid, seq) {
            e_m().addArg( e_m().Individual(to_string(nid, ts_)) );
         }
         TDLConceptExpression* ce = e_m().OneOf();
         e_m().addArg(ce);
      }
      return k_.equalConcepts();

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported operation")
            << Err::str1_t(to_string(op, ts_))
   );
   }
}

/*
*******************************************************************************/
TDLAxiom* Adaptor_triple::axiom_custom_predicate(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();

   if( ! is_iri(ts_[pred].ns_id() ) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("non-IRI predicate in x *:y z")
            << Err::str1_t(to_string(pred, ts_))
   );

   const Node_property np = declaration<Node_property>(pred, ts_);

   if( np.is_annotation() ) return 0;

   if( np.is_object() ) {
      return k_.relatedTo( obj_value(subj), obj_property(pred), obj_value(obj) );
   }

   if( is_empty(ts_[obj].ns_id()) && np.is_data() ) {
      return k_.valueOf( obj_value(subj), data_property(pred), data_value(obj) );
   }

   BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown predicate type")
            << Err::str1_t(to_string(pred, ts_))
      );
}

/*
*******************************************************************************/
void Adaptor_triple::submit_custom_triple(Triple const& t) {
   const Node_id subj = t.subject();
   const Node_id pred = t.predicate();
   const Node_id obj = t.object();
   const Node_property np = declaration<Node_property>(pred, ts_);
   if( np.is_object() ) {
      k_.relatedTo(
               obj_value(subj),
               obj_property(pred),
               obj_value(obj)
      );
      return;
   }

   if(np.is_data() ) {
      k_.valueOf(
               obj_value(subj),
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
            << Err::str1_t(to_string(pred, ts_))
      );
}

/*
*******************************************************************************/
TDLConceptExpression* Adaptor_triple::obj_type(const Node_id nid) {
   return make_expression<Obj_type>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLIndividualExpression* Adaptor_triple::obj_value(const Node_id nid) {
   Node const& node = ts_[nid];
   if( is_blank(node.ns_id()) || is_empty(node.ns_id()) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("invalid node for object instance declaration")
            << Err::str1_t(to_string(nid, ts_))
   );
   return e_m().Individual(to_string(nid, ts_));
}

/*
*******************************************************************************/
TDLObjectRoleExpression* Adaptor_triple::obj_property(const Node_id nid) {
   return make_expression<Obj_prop>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLDataRoleExpression* Adaptor_triple::data_property(const Node_id nid) {
   return make_expression<Data_prop>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLDataTypeExpression* Adaptor_triple::data_type(const Node_id nid) {
   return make_expression<Data_type>( nid, ts_ )->get(k_);
}

/*
*******************************************************************************/
TDLDataValue const* Adaptor_triple::data_value(const Node_id nid) {
   Node_literal const& node = to_literal(ts_[nid]);
   if( node.ns_id() != N_empty::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("literal node is expected")
            << Err::str1_t(to_string(nid, ts_))
   );
   const Node_id dt = node.datatype();
   return e_m().DataValue(node.value_str(), data_type(dt));
}

/*
*******************************************************************************/
TExpressionManager& Adaptor_triple::e_m() {
   return *k_.getExpressionManager();
}

/*
*******************************************************************************/
TDLAxiom* Adaptor_triple::negative_property_assertion(const Node_id nid) {
   Triple_store::result_b<1,1,0,0>::type r1 =
            ts_.find_triple(nid, T_owl_sourceIndividual::id(), any(), any());
   if( ! r1 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no owl:sourceIndividual in owl:NegativePropertyAssertion")
            << Err::str1_t(to_string(nid, ts_))
   );
   const Node_id src_ind = r1.front().object();

   Triple_store::result_b<1,1,0,0>::type r2 =
            ts_.find_triple(nid, T_owl_assertionProperty::id(), any(), any());
   if( ! r2 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no owl:assertionProperty in owl:NegativePropertyAssertion")
            << Err::str1_t(to_string(nid, ts_))
            << Err::str2_t(to_string(src_ind, ts_))
   );
   const Node_id prop = r2.front().object();
   const Node_property nt = declaration<Node_property>(prop, ts_);
   if( ! nt.is_object() && ! nt.is_data() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("undefined property in owl:NegativePropertyAssertion")
            << Err::str1_t(to_string(prop, ts_))
   );
   const Node_id tiv = nt.is_object() ?
            T_owl_targetIndividual::id() :
            T_owl_targetValue::id();
   Triple_store::result_b<1,1,0,0>::type r3 =
            ts_.find_triple(nid, tiv, any(), any());
   if( ! r3 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no owl:target* in owl:NegativePropertyAssertion")
            << Err::str1_t(to_string(nid, ts_))
            << Err::str2_t(to_string(src_ind, ts_))
            << Err::str3_t(to_string(prop, ts_))
   );
   const Node_id target = r3.front().object();

   if( nt.is_object() ) {
      return k_.relatedToNot(obj_value(src_ind), obj_property(prop), obj_value(target));
   } else {
      return k_.valueOfNot(obj_value(src_ind), data_property(prop), data_value(target));
   }
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
