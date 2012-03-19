/** @file "/owlcpp/lib/logic/expressions_ot.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXPRESSIONS_OT_HPP_
#define EXPRESSIONS_OT_HPP_
#include <vector>
#include <string>
#include "boost/foreach.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "expressions.hpp"
#include "owlcpp/rdf/query_triples.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/**@brief
*******************************************************************************/
class Ot_declared : public Fact_expression<Obj_type> {
public:
   Ot_declared(Expression_args const& ea, Triple_store const& ts)
   : iri_(ts.string(ea.handle)) {
      Node const& node = ts[ea.handle];
      if( is_blank(node.ns_id()) || is_empty(node.ns_id()) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid node for object type declaration")
               << Err::str1_t(ts.string(ea.handle))
      );
   }

   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Concept(iri_);
   }

private:
   const std::string iri_;
};

/**@brief
*******************************************************************************/
class Ot_complement : public Fact_expression<Obj_type> {
public:
   Ot_complement(Expression_args const& ea, Triple_store const& ts)
   : ot_(make_fact_expression<Obj_type>(ea.obj1, ts))
   {
      if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong rdf:type for owl:complementOf expression")
               << Err::str1_t(ts.string(ea.e_type))
      );
   }

   TDLConceptExpression* get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Not(ot_->get(k));
   }

private:
   ptr_t ot_;
};

/**@brief
*******************************************************************************/
class Ot_card : public Fact_expression<Obj_type> {
public:
   Ot_card(Expression_args const& ea, Triple_store const& ts)
   : op_( make_fact_expression<Obj_prop>(ea.obj1, ts) ),
     ot_(
              is_empty(ea.obj2) ?
                       Fact_expression<Obj_type>::ptr_t() :
                       make_fact_expression<Obj_type>(ea.obj2, ts)
     ),
     card_type_(ea.card_type),
     n_(ea.cardinality)
   {
      if( ea.e_type != T_owl_Restriction::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong rdf:type for owl:*cardinality expression")
               << Err::str1_t(ts.string(ea.e_type))
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
                     << Err::msg_t("unsupported cardinality type")
                     << Err::str1_t(ts.string(card_type_))
            );
      }
   }

   TDLConceptExpression* get(ReasoningKernel& k ) const {
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

private:
   Fact_expression<Obj_prop>::ptr_t op_;
   Fact_expression<Obj_type>::ptr_t ot_;
   const Node_id card_type_;
   const unsigned n_;
};

/**@brief Type from a list of types (owl:intersectionOf, owl:unionOf)
*******************************************************************************/
class Ot_tlist : public Fact_expression<Obj_type> {
public:
   Ot_tlist(Expression_args const& ea, Triple_store const& ts)
   : type_(ea.pred1)
   {
      if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unexpected rdf:type; should be owl:Class")
               << Err::str1_t(ts.string(ea.e_type))
      );

      BOOST_FOREACH(const Node_id nid, rdf_list(ea.obj1, ts)) {
         Node const& node = ts[nid];
         if( is_empty(node.ns_id()) ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("invalid node for object type declaration")
                  << Err::str1_t(ts.string(nid))
         );
         otl_.push_back(make_fact_expression<Obj_type>(nid, ts));
      }
      if( otl_.size() < 2 ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("at least two object type expressions is required")
               << Err::str1_t(ts.string(type_))
               << Err::str2_t(ts.string(ea.obj1))
      );
      switch(type_()) {
      case T_owl_intersectionOf::index:
      case T_owl_unionOf::index:
         break;
      default:
         BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("unsupported predicate")
                     << Err::str1_t(ts.string(type_))
            );
      }
   }

   TDLConceptExpression* get(ReasoningKernel& k ) const {
      TExpressionManager& em = *k.getExpressionManager();
      em.newArgList();
      BOOST_FOREACH(Fact_expression<Obj_type> const& ote, otl_) em.addArg( ote.get(k) );
      switch(type_()) {
      case T_owl_intersectionOf::index:
         return em.And();
      case T_owl_unionOf::index:
         return em.Or();
      default:
         return 0;//shouldn't come to this
      }
   }

private:
   const Node_id type_;
   boost::ptr_vector<Fact_expression<Obj_type> > otl_;  /**< object type expressions list */
};

/**@brief generate owl:oneOf type expression
*******************************************************************************/
class Ot_ilist : public Fact_expression<Obj_type> {
public:
   Ot_ilist(Expression_args const& ea, Triple_store const& ts)
   {
      if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unexpected rdf:type; should be owl:Class")
               << Err::str1_t(ts.string(ea.e_type))
      );
      BOOST_FOREACH(const Node_id nid, rdf_list(ea.obj1, ts)) {
         Node const& node = ts[nid];
         if( is_blank(node.ns_id()) || is_empty(node.ns_id()) ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("invalid node for instance declaration")
                  << Err::str1_t(ts.string(nid))
         );
         il_.push_back(ts.string(nid));
      }
   }

   TDLConceptExpression* get(ReasoningKernel& k ) const {
      TExpressionManager& em = *k.getExpressionManager();
      if( il_.empty() ) return em.Bottom();
      em.newArgList();
      BOOST_FOREACH(std::string const& iri, il_) em.addArg( em.Individual(iri) );
      return em.OneOf();
   }

private:
   std::vector<std::string> il_; /**< instance IRI list */
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* EXPRESSIONS_OT_HPP_ */
