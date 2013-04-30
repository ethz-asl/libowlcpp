/** @file "/owlcpp/lib/logic/factpp/data_range.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "data_range.hpp"
#include "data_instance.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_triples.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
Facet_restriction::Facet_restriction(
         const Node_id facet,
         Node_literal const& val,
         Triple_store const& ts
)
: facet_(facet),
  val_(new D_value(val, ts))
{
   switch(facet_()) {
   case xsd_maxExclusive::index:
   case xsd_maxInclusive::index:
   case xsd_minExclusive::index:
   case xsd_minInclusive::index:
      break;
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported facet")
            << Err::str1_t(to_string(facet, ts))
   );
   /* no break */
   }
}

/*
*******************************************************************************/
Facet_restriction::generated_t Facet_restriction::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   const TDLDataValue* val = val_->get(k);
   switch(facet_()) {
   case xsd_maxExclusive::index: return em.FacetMaxExclusive(val);
   case xsd_maxInclusive::index: return em.FacetMaxInclusive(val);
   case xsd_minExclusive::index: return em.FacetMinExclusive(val);
   case xsd_minInclusive::index: return em.FacetMinInclusive(val);
   default: BOOST_THROW_EXCEPTION( Err() << Err::msg_t("unsupported facet") );
      /* no break */
   }
}

/*
*******************************************************************************/
Dt_restriction::Dt_restriction(
         Expression_args const& ea, Triple_store const& ts
) : dt_(make_expression<Data_type>(ea.obj1, ts))
{
   BOOST_ASSERT(ea.pred1 == owl_onDatatype::id());
   BOOST_ASSERT(ea.pred2 == owl_withRestrictions::id());
   BOOST_FOREACH(Node_id const nid, rdf_list(ea.obj2, ts)) {
      facets_.push_back(make_expression<Data_facet>(nid, ts));
   }
   if( facets_.empty() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("no facets in datatype restriction")
   );
}

/*
*******************************************************************************/
Dt_restriction::generated_t Dt_restriction::get(ReasoningKernel& k ) const {
   BOOST_ASSERT( ! facets_.empty() );
   TExpressionManager& em = *k.getExpressionManager();
   Expression<Data_type>::generated_t e = dt_->get(k);
   for(std::size_t n = 0; n != facets_.size(); ++n) {
      e = em.RestrictedType(e, facets_[n].get(k));
   }
   return e;
}

/*
*******************************************************************************/
Dt_junction::Dt_junction(Expression_args const& ea, Triple_store const& ts)
: nid_(ea.pred1)
{
   if( ea.e_type != rdfs_Datatype::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("rdfs:Datatype is expected")
            << Err::str1_t(to_string(ea.e_type, ts))
   );

   switch(nid_()) {
   case owl_intersectionOf::index:
   case owl_unionOf::index:
      break;
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("owl:intersectionOf or owl:unionOf is expected")
            << Err::str1_t(to_string(nid_, ts))
   );
   /* no break */
   }

   BOOST_FOREACH(Node_id const nid, rdf_list(ea.obj1, ts)) {
      l_.push_back(make_expression<Data_range>(nid, ts));
   }
   if( l_.size() < 2 ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("at least 2 data ranges should be provided")
   );
}

/*
*******************************************************************************/
Dt_junction::generated_t Dt_junction::get(ReasoningKernel& k ) const {
   BOOST_ASSERT(nid_ == owl_intersectionOf::id() || nid_ == owl_unionOf::id() );
   BOOST_ASSERT( l_.size() >= 2U );
   TExpressionManager& em = *k.getExpressionManager();
   em.newArgList();
   for(std::size_t n = 0; n != l_.size(); ++n) em.addArg(l_[n].get(k));

   switch(nid_()) {
   case owl_intersectionOf::index: return em.DataAnd();
   case owl_unionOf::index: return em.DataOr();
      break;
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("owl:intersectionOf or owl:unionOf is expected")
   );
   /* no break */
   }
}

/*
*******************************************************************************/
Dt_oneof::Dt_oneof(Expression_args const& ea, Triple_store const& ts) {
   BOOST_FOREACH(Node_id const nid, rdf_list(ea.obj1, ts)) {
      l_.push_back(make_expression<Data_inst>(nid, ts));
   }
   if( l_.empty() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("at least 1 data value should be provided")
   );
}

/*
*******************************************************************************/
Dt_oneof::generated_t Dt_oneof::get(ReasoningKernel& k ) const {
   BOOST_ASSERT( ! l_.empty() );
   TExpressionManager& em = *k.getExpressionManager();
   em.newArgList();
   for(std::size_t n = 0; n != l_.size(); ++n) em.addArg(l_[n].get(k));
   return em.DataOneOf();
}

/*
*******************************************************************************/
Dt_complement::Dt_complement(Expression_args const& ea, Triple_store const& ts)
: de_(make_expression<Data_range>(ea.obj1, ts))
{}

/*
*******************************************************************************/
Dt_complement::generated_t Dt_complement::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   return em.DataNot(de_->get(k));
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
