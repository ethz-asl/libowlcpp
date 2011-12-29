/** @file "/owlcpp/lib/fact_visitor.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
//#include "pch.hpp"
#include <cassert>
#include "fact_visitor.hpp"
#include "boost/foreach.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_nodes.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "../str_conversion.hpp"

namespace owlcpp {

namespace{
typedef Triples_query<Node_id, Node_id, blank>::q_range_t q_range_nnb_t;
}//namespace anonymous

/* Annotation or property assertion
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_empty_generic>& tr) {
   if( tr.predicate() == ot::T_rdf_type::id() ) {
      instance(tr.subject(), tr.object());
      return;
   }

   const Triple& tr2 = find_first_triple(
               tr.predicate(), ot::T_rdf_type::id(), blank(), store_
         );

   switch (tr2.object()()) {
   case ot::T_owl_AnnotationProperty::index: //do nothing
      return;
   case ot::T_owl_AsymmetricProperty::index:
   case ot::T_owl_FunctionalProperty::index:
   case ot::T_owl_InverseFunctionalProperty::index:
   case ot::T_owl_IrreflexiveProperty::index:
   case ot::T_owl_ObjectProperty::index:
   case ot::T_owl_ReflexiveProperty::index:
   case ot::T_owl_SymmetricProperty::index:
   case ot::T_owl_TransitiveProperty::index: {
      const TDLIndividualExpression* i1 = instance( tr.subject() );
      const TDLObjectRoleExpression* p = property( tr.predicate() );
      const TDLIndividualExpression* i2 = instance( tr.object() );
      kernel_.relatedTo( i1, p, i2 );
      return;
   }
   default:
      BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown predicate type")
            << Err::str1_t(short_name(tr2.object(), store_))
      );
   }
}

/* @param tr {_:x, rdf:type, owl:AllDifferent},
    {_:x owl:members Seq} or {_:x owl:distinctMembers Seq}
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_AllDifferent>& tr) {
   assert(tr.predicate()() == ot::T_rdf_type::index);
   assert(tr.object()() == ot::T_owl_AllDifferent::index);

   q_range_nnb_t range = find_triples(
         tr.subject(),
         ot::T_owl_members::id(),
         blank(),
         store_
   );

   if( ! range ) range = find_triples(
         tr.subject(),
         ot::T_owl_distinctMembers::id(),
         blank(),
         store_
   );

   if( ! range ) BOOST_THROW_EXCEPTION(
         Err()
         << Err::msg_t("found owl:AllDifferent but neither "
               "owl:members nor owl:distinctMembers")
   );

   const std::vector<Node_id> c = rdf_collection(range.front().get<2>(), store_);
   e_manager().newArgList();
   BOOST_FOREACH(const Node_id nid, c) e_manager().addArg( instance(nid) );
   kernel_.processDifferent();
}


/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_AllDisjointClasses>& tr) {
   assert(tr.predicate()() == ot::T_rdf_type::index);
   assert(tr.object()() == ot::T_owl_AllDisjointClasses::index);

   const Node_id seq_id =
         find_first_triple(
               tr.subject(), ot::T_owl_members::id(), blank(), store_
         ).object();

   const std::vector<Node_id> c = rdf_collection(seq_id, store_);

   e_manager().newArgList();
   BOOST_FOREACH(const Node_id nid, c) e_manager().addArg( concept(nid) );

   kernel_.disjointConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_AllDisjointProperties>& tr) {
   assert(tr.predicate()() == ot::T_rdf_type::index);
   assert(tr.object()() == ot::T_owl_AllDisjointProperties::index);

   const Node_id seq_id =
         find_first_triple(
               tr.subject(), ot::T_owl_members::id(), blank(), store_
         ).object();

   const std::vector<Node_id> c = rdf_collection(seq_id, store_);

   e_manager().newArgList();
   BOOST_FOREACH(const Node_id nid, c) e_manager().addArg( property(nid) );

   kernel_.disjointORoles();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_allValuesFrom>& tr) {
   const Node_id property_id =
         find_first_triple(
               tr.subject(), ot::T_owl_onProperty::id(), blank(), store_
         ).object();

   const TDLConceptExpression* restriction =
         e_manager().Forall( property(property_id), concept( tr.object() ) );
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( restriction );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_cardinality>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const unsigned n = get_unsigned(store_[tr.object()].value_str());

   const TDLConceptExpression* restriction =
         e_manager().Cardinality(n, pe,  e_manager().Top());

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( restriction );
   kernel_.equalConcepts();
}


/* not
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_complementOf>& tr) {
   const TDLConceptExpression* not_obj_e =
         e_manager().Not( concept( tr.object() ) );
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( not_obj_e );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_datatypeComplementOf>& tr) {
   //TODO:
   BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("\"owl:datatypeComplementOf\" not supported"));
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_differentFrom>& tr) {
   e_manager().newArgList();
   e_manager().addArg( instance( tr.subject() ) );
   e_manager().addArg( instance( tr.object() ) );
   kernel_.processDifferent();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_disjointUnionOf>& tr) {
   const std::vector<Node_id> c = rdf_collection(tr.object(), store_);

   e_manager().newArgList();
   BOOST_FOREACH(const Node_id nid, c) e_manager().addArg( concept(nid) );

   kernel_.disjointUnion( concept(tr.subject()) );
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_disjointWith>& tr) {
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( concept( tr.object() ) );
   kernel_.disjointConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_equivalentClass>& tr) {
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( concept( tr.object() ) );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_equivalentProperty>& tr) {
   e_manager().newArgList();
   e_manager().addArg( property( tr.subject() ) );
   e_manager().addArg( property( tr.object() ) );
   kernel_.equalORoles();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_hasKey>& tr) {
   //FaCT++ does not support
   BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("\"owl:hasKey\" not supported"));
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_hasSelf>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);
   const TDLConceptExpression* r = e_manager().SelfReference(pe);

   const bool has_self = get_bool(store_[tr.object()].value_str());

   if( ! has_self ) r = e_manager().Not(r);
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( r );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_hasValue>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const TDLIndividualExpression* ind = instance(tr.object());

   const TDLConceptExpression* r = e_manager().Value(pe, ind);

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( r );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_intersectionOf>& tr) {
   e_manager().newArgList();
   const std::vector<Node_id> c = rdf_collection(tr.object(), store_);
   BOOST_FOREACH(const Node_id nid, c) {
      e_manager().addArg( concept(nid) );
   }
   const TDLConceptExpression* intersection_e = e_manager().And();
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( intersection_e );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_maxCardinality>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const unsigned n = get_unsigned(store_[tr.object()].value_str());

   const TDLConceptExpression* restriction =
         e_manager().MaxCardinality(n, pe,  e_manager().Top());

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( restriction );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_maxQualifiedCardinality>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const Node_id cid =
         find_first_triple(tr.subject(), ot::T_owl_onClass::id(), blank(), store_).object();
   const TDLConceptExpression* ce = concept(cid);

   const unsigned n = get_unsigned(store_[tr.object()].value_str());

   const TDLConceptExpression* r = e_manager().MaxCardinality(n, pe, ce);

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( r );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_minCardinality>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const unsigned n = get_unsigned(store_[tr.object()].value_str());

   const TDLConceptExpression* restriction =
         e_manager().MinCardinality(n, pe,  e_manager().Top());

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( restriction );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_minQualifiedCardinality>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const Node_id cid =
         find_first_triple(tr.subject(), ot::T_owl_onClass::id(), blank(), store_).object();
   const TDLConceptExpression* ce = concept(cid);

   const unsigned n = get_unsigned(store_[tr.object()].value_str());

   const TDLConceptExpression* r = e_manager().MaxCardinality(n, pe, ce);

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( r );
   kernel_.equalConcepts();
}

/**
@details process four triples
_:x rdf:type owl:NegativePropertyAssertion .
_:x owl:sourceIndividual T(a1) .
_:x owl:assertionProperty T(OPE) .
_:x owl:targetIndividual T(a2)
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_NegativePropertyAssertion>& tr) {
   Node_id target_id;
   Node_id source_id;
   Node_id property_id;
   BOOST_FOREACH(
         const Triple& t,
         find_triples(tr.subject(), blank(), blank(), store_)
   ) {
      switch (t.predicate()()) {
         case ot::T_rdf_type::index :
            break;
         case ot::T_owl_targetIndividual::index :
            target_id = t.object();
            break;
         case ot::T_owl_sourceIndividual::index :
            source_id = t.object();
            break;
         case ot::T_owl_assertionProperty::index :
            property_id = t.object();
            break;
         default:
            BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unexpected predicate")
                  << Err::str1_t(short_name(t.predicate(), store_))
            );
      }
   }
   assert(target_id > Node_id());
   assert(source_id > Node_id());
   assert(property_id > Node_id());
   kernel_.relatedToNot(
         instance(source_id),
         property(property_id),
         instance(target_id)
   );
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_oneOf>& tr) {
   e_manager().newArgList();
   const std::vector<Node_id> c = rdf_collection(tr.object(), store_);
   BOOST_FOREACH(const Node_id nid, c) {
      e_manager().addArg( instance(nid) );
   }
   const TDLConceptExpression* e = e_manager().OneOf();
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( e );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_propertyChainAxiom>& tr) {
   const std::vector<Node_id> c = rdf_collection(tr.object(), store_);

   e_manager().newArgList();
   BOOST_FOREACH(const Node_id nid, c) e_manager().addArg( property(nid) );
   const TDLObjectRoleComplexExpression* p = e_manager().Compose();

   kernel_.impliesORoles(p, property( tr.subject() ));
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_propertyDisjointWith>& tr) {
   e_manager().newArgList();
   e_manager().addArg( property(tr.subject()) );
   e_manager().addArg( property(tr.object()) );
   kernel_.disjointORoles();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_qualifiedCardinality>& tr) {
   const Node_id pid =
         find_first_triple(tr.subject(), ot::T_owl_onProperty::id(), blank(), store_).object();
   const TDLObjectRoleExpression* pe = property(pid);

   const Node_id cid =
         find_first_triple(tr.subject(), ot::T_owl_onClass::id(), blank(), store_).object();
   const TDLConceptExpression* ce = concept(cid);

   const unsigned n = get_unsigned(store_[tr.object()].value_str());

   const TDLConceptExpression* r = e_manager().Cardinality(n, pe, ce);

   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( r );
   kernel_.equalConcepts();
}


/* same individual
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_sameAs>& tr) {
   e_manager().newArgList();
   e_manager().addArg( instance( tr.subject() ) );
   e_manager().addArg( instance( tr.object() ) );
   kernel_.processSame();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_someValuesFrom>& tr) {
   const Node_id property_id =
         find_first_triple(
               tr.subject(), ot::T_owl_onProperty::id(), blank(), store_
         ).object();

   const TDLConceptExpression* restriction =
         e_manager().Exists( property(property_id), concept( tr.object() ) );
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( restriction );
   kernel_.equalConcepts();
}

/* Subject equivalent to union of classes in object list
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_unionOf>& tr) {
   e_manager().newArgList();
   const std::vector<Node_id> c = rdf_collection(tr.object(), store_);
   BOOST_FOREACH(const Node_id nid, c) {
      e_manager().addArg( concept(nid) );
   }
   const TDLConceptExpression* union_e = e_manager().Or();
   e_manager().newArgList();
   e_manager().addArg( concept( tr.subject() ) );
   e_manager().addArg( union_e );
   kernel_.equalConcepts();
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_owl_withRestrictions>& tr) {
   //TODO:
   BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("\"owl:withRestrictions\" not supported"));
}

/*
*******************************************************************************/
void Triple_to_fact::Visit(const Triple_std<ot::T_rdfs_Datatype>& tr) {
   //TODO:
   BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("\"rdfs:Datatype\" not supported"));
}

/*
*******************************************************************************/
TDLIndividualExpression* Triple_to_fact::instance(const Node_id i, const Node_id c) {
   if(
         find_triples(
               c,
               ot::T_rdf_type::id(),
               ot::T_owl_Class::id(),
               store_
         )
   ) {
      TDLIndividualExpression* inst = instance( i );
      const TDLConceptExpression* conc = concept( c );
      kernel_.instanceOf(inst, conc);
      return inst;
   } else {
      BOOST_THROW_EXCEPTION( Err() << Err::msg_t("type is undeclared") );
   }
}

/*
*******************************************************************************/
}//namespace owlcpp
