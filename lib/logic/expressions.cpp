/** @file "/owlcpp/lib/logic/expressions.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/

#include "expressions.hpp"
#include "expressions_ot.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/*
*******************************************************************************/
template<>typename Fact_expression<Obj_class>::ptr_t
make_fact_expression<Obj_class>(Expression_args const& ea, Triple_store const& ts) {
   typedef typename Fact_expression<Obj_class>::ptr_t ptr_t;
   typedef typename Fact_expression<Obj_class>::Err Err;
   if( ! is_blank(ts[ea.handle].ns_id()) )
      return ptr_t(new Ot_declared(ea, ts));
   switch (ea.card_type()) {
      case T_owl_cardinality::index:
      case T_owl_maxCardinality::index:
      case T_owl_minCardinality::index:
      case T_owl_maxQualifiedCardinality::index:
      case T_owl_minQualifiedCardinality::index:
      case T_owl_qualifiedCardinality::index:
         return ptr_t(new Ot_card(ea, ts));
      default:
         break;
   }

   switch (ea.pred2()) {
   case T_owl_allValuesFrom::index:
   case T_owl_hasValue::index:
   case T_owl_hasSelf::index:
   case T_owl_someValuesFrom::index:

      break;
   }

   switch (ea.pred1()) {
   case T_owl_intersectionOf::index:
   case T_owl_unionOf::index:
      return ptr_t(new Ot_tlist(ea, ts));
   case T_owl_complementOf::index:
      return ptr_t(new Ot_complement(ea, ts));
   case T_owl_oneOf::index:
      return ptr_t(new Ot_ilist(ea, ts));

      break;
   }
   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported object type")
               << Err::str1_t(ts.string(ea.handle))
   );
}

/*
*******************************************************************************/

}//namespace detail
}//namespace owlcpp
