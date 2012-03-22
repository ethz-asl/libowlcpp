/** @file "/owlcpp/lib/logic/factpp/expression.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/

#include "expression.hpp"
#include "obj_type.hpp"
#include "obj_property.hpp"
#include "owlcpp/rdf/query_node.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
template<>typename Expression<Obj_type>::ptr_t
make_expression<Obj_type>(Expression_args const& ea, Triple_store const& ts) {
   typedef typename Expression<Obj_type>::ptr_t ptr_t;
   typedef typename Expression<Obj_type>::Err Err;

   if( ea.handle == T_owl_Thing::id() ) return ptr_t(new Ot_thing());

   if( ea.handle == T_owl_Nothing::id() ) return ptr_t(new Ot_nothing());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Ot_declared(ea, ts));

   switch (ea.cardinality()) {
      case T_owl_cardinality::index:
      case T_owl_maxCardinality::index:
      case T_owl_minCardinality::index:
      case T_owl_maxQualifiedCardinality::index:
      case T_owl_minQualifiedCardinality::index:
      case T_owl_qualifiedCardinality::index:
         return ptr_t(new Ot_card_restriction(ea, ts));
      default:
         break;
   }

   switch (ea.pred2()) {
   case T_owl_allValuesFrom::index:
   case T_owl_hasValue::index:
   case T_owl_hasSelf::index:
   case T_owl_someValuesFrom::index:
         return ptr_t(new Ot_restriction(ea, ts));
   default:
      break;
   }

   switch (ea.pred1()) {
   case T_owl_intersectionOf::index:
   case T_owl_unionOf::index:
      return ptr_t(new Ot_type_list(ea, ts));
   case T_owl_complementOf::index:
      return ptr_t(new Ot_complement(ea, ts));
   case T_owl_oneOf::index:
      return ptr_t(new Ot_instance_list(ea, ts));

      break;
   }

   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported object type")
               << Err::str1_t(to_string_short(ea.handle, ts))
   );
}

/*
*******************************************************************************/
template<> typename Expression<Obj_prop>::ptr_t
make_expression<Obj_prop>(Expression_args const& ea, Triple_store const& ts) {
   typedef typename Expression<Obj_prop>::ptr_t ptr_t;
   typedef typename Expression<Obj_prop>::Err Err;

   if( ea.handle == T_owl_topObjectProperty::id() ) return ptr_t(new Op_top());
   if( ea.handle == T_owl_bottomObjectProperty::id() ) return ptr_t(new Op_bottom());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Op_declared(ea, ts));

}

/*
*******************************************************************************/
template<> typename Expression<Data_type>::ptr_t
make_expression<Data_type>(Expression_args const& ea, Triple_store const& ts) {
   typedef typename Expression<Data_type>::ptr_t ptr_t;
   typedef typename Expression<Data_type>::Err Err;

}

/*
*******************************************************************************/
template<> typename Expression<Data_prop>::ptr_t
make_expression<Data_prop>(Expression_args const& ea, Triple_store const& ts) {
   typedef typename Expression<Data_prop>::ptr_t ptr_t;
   typedef typename Expression<Data_prop>::Err Err;

}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
