/** @file "/owlcpp/lib/logic/expressions_ot.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXPRESSIONS_OT_HPP_
#define EXPRESSIONS_OT_HPP_
#include <list>
#include "boost/foreach.hpp"
#include "expressions.hpp"
#include "owlcpp/rdf/query_triples.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/**@brief
*******************************************************************************/
class Ot_declared : public Obj_type {
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

   TDLConceptExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      return k.getExpressionManager()->Concept(iri_);
   }

private:
   const std::string iri_;
};

/**@brief
*******************************************************************************/
class Ot_complement : public Obj_type {
public:
   Ot_complement(Expression_args const& ea, Triple_store const& ts)
   : ot_(Obj_type::make(ea.obj1, ts))
   {
      if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong rdf:type for owl:complementOf expression")
               << Err::str1_t(ts.string(ea.e_type))
      );
   }

   TDLConceptExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      return k.getExpressionManager()->Not(ot_->get(ts, k));
   }

private:
   ptr_t ot_;
};

/**@brief
*******************************************************************************/
class Ot_card : public Obj_type {
public:
   Ot_card(Expression_args const& ea, Triple_store const& ts)
   : op_( Obj_prop::make(ea.obj1, ts) ),
     ot_(is_empty(ea.obj2) ? Obj_type::ptr_t() : Obj_type::make(ea.obj2, ts)),
     card_type_(ea.card_type),
     n_(ea.cardinality)
   {
      if( ea.e_type != T_owl_Restriction::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong rdf:type for owl:*cardinality expression")
               << Err::str1_t(ts.string(ea.e_type))
      );
   }

   TDLConceptExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      TExpressionManager& em = *k.getExpressionManager();
      switch(card_type_()) {
      case T_owl_cardinality::index:
         return em.Cardinality(n_, op_->get(ts, k), em.Top());
      case T_owl_maxCardinality::index:
         return em.MaxCardinality(n_, op_->get(ts, k), em.Top());
      case T_owl_minCardinality::index:
         return em.MinCardinality(n_, op_->get(ts, k), em.Top());
      case T_owl_maxQualifiedCardinality::index:
         return em.MaxCardinality(n_, op_->get(ts, k), ot_->get(ts, k));
      case T_owl_minQualifiedCardinality::index:
         return em.MinCardinality(n_, op_->get(ts, k), ot_->get(ts, k));
      case T_owl_qualifiedCardinality::index:
         return em.Cardinality(n_, op_->get(ts, k), ot_->get(ts, k));
      default:
         BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("unsupported cardinality type")
                     << Err::str1_t(ts.string(card_type_))
            );
      }
   }

private:
   Obj_prop::ptr_t op_;
   Obj_type::ptr_t ot_;
   const Node_id card_type_;
   const unsigned n_;
};

/**@brief Type from a list of types (owl:intersectionOf, owl:unionOf)
*******************************************************************************/
class Ot_tlist : public Obj_type {
public:
   Ot_tlist(Expression_args const& ea, Triple_store const& ts)
   : ot_(Obj_type::make(ea.obj1, ts))
   {
      if( ea.e_type != T_owl_Class::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unexpected rdf:type; should be owl:Class")
               << Err::str1_t(ts.string(ea.e_type))
      );
   }

   TDLConceptExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      TExpressionManager& em = *k.getExpressionManager();
      switch(type_()) {
      case T_owl_intersectionOf::index:
      case T_owl_unionOf::index:
         return em.Cardinality(n_, op_->get(ts, k), em.Top());
      default:
         BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("unsupported predicate")
                     << Err::str1_t(ts.string(type_))
            );
      }
   }

private:
   ptr_t ot_;
   const Node_id type_;
};

/**@brief generate owl:oneOf type expression
*******************************************************************************/
class Ot_olist : public Obj_type {
public:
   Ot_olist(Expression_args const& ea, Triple_store const& ts)
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
                  << Err::msg_t("invalid node for object type declaration")
                  << Err::str1_t(ts.string(nid))
         );
         o_.push_back(ts.string(nid));
      }
   }

   TDLConceptExpression* get(Triple_store const& ts, ReasoningKernel& k ) const {
      TExpressionManager& em = *k.getExpressionManager();
      if( o_.empty() ) return em.Bottom();
   }

private:
   std::list<std::string> o_;
};

}//namespace detail
}//namespace owlcpp
#endif /* EXPRESSIONS_OT_HPP_ */
