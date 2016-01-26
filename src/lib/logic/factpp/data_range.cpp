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
#include "fact++/Kernel.h"

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
Dr_standard::Dr_standard(Expression_args const& ea, Triple_store const& ts)
: nid_(ea.handle)
{
   switch( internal_type_id(nid_) ) {
   case detail::Top_tid:
   case detail::Bool_tid:
   case detail::Int_tid:
   case detail::Unsigned_tid:
   case detail::Double_tid:
   case detail::Time_tid:
   case detail::String_tid:
      break;
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown standard datatype")
            << Err::str1_t(ea.string(ts))
   );
   /* no break */
   }

}

/*
*******************************************************************************/
Dr_standard::generated_t Dr_standard::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   switch( internal_type_id(nid_) ) {
   case detail::Top_tid: return em.DataTop();
   case detail::Bool_tid: return em.getBoolDataType();
   case detail::Int_tid:
   case detail::Unsigned_tid: return em.getIntDataType();
   case detail::Double_tid: return em.getRealDataType();
   case detail::Time_tid: return em.getTimeDataType();
   case detail::String_tid:
   default: return em.getStrDataType();
   }
}

/*
*******************************************************************************/
Dt_restriction::Dt_restriction(
         Expression_args const& ea, Triple_store const& ts
) : dt_(make_expression<Data_type>(ea.obj1, ts))
{
   if(
            ea.e_type != rdfs_Datatype::id() ||
            ea.pred1 != owl_onDatatype::id() ||
            ea.pred2 != owl_withRestrictions::id()
   ) {
      char const* msg =
               "_:x rdf:type rdfs:Datatype; "
               "_:x owl:onDatatype *:y; "
               "_:x owl:withRestrictions "
               "is expected";
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t(msg)
               << Err::str1_t(ea.string(ts))
   );
   }
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
            << Err::msg_t("rdf:type rdfs:Datatype is expected")
            << Err::str1_t(ea.string(ts))
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
   if( ea.e_type != rdfs_Datatype::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("rdf:type rdfs:Datatype is expected")
            << Err::str1_t(ea.string(ts))
   );
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
{
   if( ea.e_type != rdfs_Datatype::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("rdf:type rdfs:Datatype is expected")
            << Err::str1_t(ea.string(ts))
   );
}

/*
*******************************************************************************/
Dt_complement::generated_t Dt_complement::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   return em.DataNot(de_->get(k));
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
