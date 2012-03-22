/** @file "/owlcpp/lib/logic/factpp/obj_type.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include "obj_type.hpp"
#include "boost/foreach.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "owlcpp/logic/detail/node_declaration.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
Ot_complement::Ot_complement(Expression_args const& ea, Triple_store const& ts)
: ot_(make_expression<Obj_type>(ea.obj1, ts))
{
   if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("wrong rdf:type for owl:complementOf expression")
            << Err::str1_t(to_string_short(ea.e_type, ts))
   );
}

/*
*******************************************************************************/
Ot_restriction::Ot_restriction(Expression_args const& ea, Triple_store const& ts)
:  restr_type_(ea.pred2),
   op_(make_expression<Obj_prop>(ea.obj1, ts))
{
   if( ea.e_type != T_owl_Restriction::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("rdf:type owl:Restriction is expected")
            << Err::str1_t(to_string_short(ea.e_type, ts))
   );

   if( ea.pred1 != T_owl_onProperty::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("x owl:onProperty y is expected")
            << Err::str1_t(to_string_short(ea.pred1, ts))
   );
   init(ea.obj1, ea.obj2, ts);
}

/*
*******************************************************************************/
void Ot_restriction::init(
         const Node_id prop,
         const Node_id val,
         Triple_store const& ts
) {
   switch(restr_type_()) {
   case T_owl_hasValue::index:
      inst_ = to_string(val, ts);
      break;
   case T_owl_hasSelf::index:
      try{ if( value<bool>(val, ts) ) break; } catch( Rdf_err const&) {}
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unexpected object in x owl:hasSelf \"true\"^^xsd:boolean")
               << Err::str1_t(to_string_short(val, ts))
      );
   case T_owl_allValuesFrom::index:
   case T_owl_someValuesFrom::index:
      check_declaration(val, Node_type::object(), ts);
      ot_ = make_expression<Obj_type>(val, ts);
      break;
   default:
      BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unsupported restriction type")
                  << Err::str1_t(to_string_short(restr_type_, ts))
         );
   }
   check_declaration(prop, Node_property::object(), ts);
   op_ = make_expression<Obj_prop>(prop, ts);
}

/*
*******************************************************************************/
TDLConceptExpression* Ot_restriction::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   switch(restr_type_()) {
   case T_owl_allValuesFrom::index:
      return em.Forall(op_->get(k), ot_->get(k));
   case T_owl_hasValue::index:
      return em.Value(op_->get(k), em.Individual(inst_));
   case T_owl_hasSelf::index:
      return em.SelfReference(op_->get(k));
   case T_owl_someValuesFrom::index:
      return em.Exists(op_->get(k), ot_->get(k));
   default:
      BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unsupported restriction type")
                  << Err::str1_t(to_string(restr_type_))
         );
   }
}

/*
*******************************************************************************/
Ot_card_restriction::Ot_card_restriction(Expression_args const& ea, Triple_store const& ts)
: op_( make_expression<Obj_prop>(ea.obj1, ts) ),
  ot_(
           is_empty(ea.obj2) ?
                    Expression<Obj_type>::ptr_t() :
                    make_expression<Obj_type>(ea.obj2, ts)
  ),
  card_type_(ea.cardinality),
  n_(ea.num)
{
   if( ea.e_type != T_owl_Restriction::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("wrong rdf:type for owl:*cardinality expression")
            << Err::str1_t(to_string_short(ea.e_type, ts))
   );
   switch(card_type_()) {
   case T_owl_cardinality::index:
   case T_owl_maxCardinality::index:
   case T_owl_minCardinality::index:
   case T_owl_maxQualifiedCardinality::index:
   case T_owl_minQualifiedCardinality::index:
   case T_owl_qualifiedCardinality::index:
      break;
   default:
      BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unsupported cardinality restriction type")
                  << Err::str1_t(to_string_short(card_type_, ts))
         );
   }
}

/*
*******************************************************************************/
TDLConceptExpression* Ot_card_restriction::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   switch(card_type_()) {
   case T_owl_cardinality::index:
      return em.Cardinality(n_, op_->get(k), em.Top());
   case T_owl_maxCardinality::index:
      return em.MaxCardinality(n_, op_->get(k), em.Top());
   case T_owl_minCardinality::index:
      return em.MinCardinality(n_, op_->get(k), em.Top());
   case T_owl_maxQualifiedCardinality::index:
      return em.MaxCardinality(n_, op_->get(k), ot_->get(k));
   case T_owl_minQualifiedCardinality::index:
      return em.MinCardinality(n_, op_->get(k), ot_->get(k));
   case T_owl_qualifiedCardinality::index:
      return em.Cardinality(n_, op_->get(k), ot_->get(k));
   default:
      return 0;//shouldn't come to this
   }
}

/*
*******************************************************************************/
Ot_type_list::Ot_type_list(Expression_args const& ea, Triple_store const& ts)
: type_(ea.pred1)
{
   if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unexpected rdf:type; should be owl:Class")
            << Err::str1_t(to_string_short(ea.e_type, ts))
   );
   make_sequence(ea.obj1, ts);
}

/*
*******************************************************************************/
void Ot_type_list::make_sequence(const Node_id nid, Triple_store const& ts) {
   switch(type_()) {
   case T_owl_intersectionOf::index:
   case T_owl_unionOf::index:
      break;
   default:
      BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unsupported predicate")
                  << Err::str1_t(to_string_short(type_, ts))
         );
   }

   const std::vector<Node_id> seq =
            boost::copy_range<std::vector<Node_id> >(rdf_list(nid, ts));

   check_seq_declaration(seq, Node_type::object(), ts);

   BOOST_FOREACH(const Node_id nid, seq) otl_.push_back(make_expression<Obj_type>(nid, ts));
}

/*
*******************************************************************************/
TDLConceptExpression* Ot_type_list::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   switch(type_()) {
   case T_owl_intersectionOf::index:
      if( otl_.empty() ) return k.getExpressionManager()->Top();
      if( otl_.size() == 1U ) return otl_[0].get(k);
      em.newArgList();
      BOOST_FOREACH(Expression<Obj_type> const& ote, otl_) em.addArg( ote.get(k) );
      return em.And();
   case T_owl_unionOf::index:
      if( otl_.empty() ) return k.getExpressionManager()->Bottom();
      if( otl_.size() == 1U ) return otl_[0].get(k);
      em.newArgList();
      BOOST_FOREACH(Expression<Obj_type> const& ote, otl_) em.addArg( ote.get(k) );
      return em.Or();
   default:
      //shouldn't come to this
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported predicate")
               << Err::str1_t(to_string(type_))
      );
   }
}

/*
*******************************************************************************/
Ot_instance_list::Ot_instance_list(Expression_args const& ea, Triple_store const& ts) {
   if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unexpected rdf:type; should be owl:Class")
            << Err::str1_t(to_string_short(ea.e_type, ts))
   );
   make_sequence(ea.obj1, ts);
}

/*
*******************************************************************************/
TDLConceptExpression* Ot_instance_list::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   if( il_.empty() ) return em.Bottom();
   em.newArgList();
   BOOST_FOREACH(std::string const& iri, il_) em.addArg( em.Individual(iri) );
   return em.OneOf();
}

/*
*******************************************************************************/
void Ot_instance_list::make_sequence(const Node_id seq, Triple_store const& ts) {
   BOOST_FOREACH(const Node_id nid, rdf_list(seq, ts)) {
      if( ! is_iri(ts[nid].ns_id()) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("non-IRI node for instance declaration")
               << Err::str1_t(to_string_short(nid, ts))
      );
      il_.push_back(to_string_short(nid, ts));
   }
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
