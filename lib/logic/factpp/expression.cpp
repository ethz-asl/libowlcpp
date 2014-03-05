/** @file "/owlcpp/lib/logic/factpp/expression.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/

#include "expression.hpp"
#include "obj_type.hpp"
#include "obj_property.hpp"
#include "data_property.hpp"
#include "data_type.hpp"
#include "data_range.hpp"
#include "data_instance.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "logic/node_property_declaration.hpp"
#include "logic/node_type_declaration.hpp"
#include "logic/find_node_declaration.hpp"
#include "owlcpp/detail/datatype_impl.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
template<> Expression<Obj_type>::ptr_t
make_expression<Obj_type>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_type>::ptr_t ptr_t;
   typedef Expression<Obj_type>::Err Err;

   if( ea.handle == owl_Thing::id() ) return ptr_t(new Ot_thing());

   if( ea.handle == owl_Nothing::id() ) return ptr_t(new Ot_nothing());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Ot_declared(ea, ts));

   if( ea.e_type == owl_Restriction::id() ) return make_restriction_ote(ea, ts);

   if( ea.e_type == owl_Class::id() ) return make_class_ote(ea, ts);

   switch (ea.pred1()) {

   case owl_intersectionOf::index:
   case owl_unionOf::index: return ptr_t(new Ot_type_list(ea, ts));

   case owl_complementOf::index: return ptr_t(new Ot_complement(ea, ts));

   case owl_oneOf::index: return ptr_t(new Ot_instance_list(ea, ts));

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported object type")
            << Err::str1_t(ea.string(ts))
   );
   /* no break */
   }
}

/*
*******************************************************************************/
template<> Expression<Obj_prop>::ptr_t
make_expression<Obj_prop>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_prop>::ptr_t ptr_t;
   typedef Expression<Obj_prop>::Err Err;

   if( ea.handle == owl_topObjectProperty::id() ) return ptr_t(new Op_top());

   if( ea.handle == owl_bottomObjectProperty::id() ) return ptr_t(new Op_bottom());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Op_declared(ea, ts));

   if( ea.pred1 == owl_inverseOf::id() ) return ptr_t(new Op_inverse(ea, ts));

   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported object property")
               << Err::str1_t(ea.string(ts))
   );
}

/*
*******************************************************************************/
template<> Expression<Data_range>::ptr_t
make_expression<Data_range>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Data_range>::ptr_t ptr_t;
   typedef Expression<Data_range>::Err Err;

   if( is_empty(ea.handle) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("data range not declared")
   );

   switch( internal_type_id(ea.handle) ) {
   case detail::Empty_tid:
   case detail::Unknown_tid:
      break;
   default: return ptr_t(new Dr_standard(ea, ts));
   }

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Dt_declared(ea, ts));

   switch(ea.pred1()) {
   case owl_intersectionOf::index:
   case owl_unionOf::index: return ptr_t(new Dt_junction(ea, ts));
   case owl_datatypeComplementOf::index: return ptr_t(new Dt_complement(ea, ts));
   case owl_oneOf::index: return ptr_t(new Dt_oneof(ea, ts));
   case owl_onDatatype::index: return ptr_t(new Dt_restriction(ea, ts));
   }
   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported data range")
               << Err::str1_t(ea.string(ts))
   );
}

/*
*******************************************************************************/
template<> Expression<Data_type>::ptr_t
make_expression<Data_type>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Data_type>::ptr_t ptr_t;
   typedef Expression<Data_type>::Err Err;

   if( is_empty(ea.handle) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("data type not declared")
   );

   if( ! is_iri(ts[ea.handle].ns_id()) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported data type")
            << Err::str1_t(ea.string(ts))
   );

   switch( internal_type_id(ea.handle) ) {
   case detail::Unknown_tid: return ptr_t(new Dt_declared(ea, ts));
   default: return ptr_t(new Dt_standard(ea, ts));
   }
}

/*
*******************************************************************************/
template<> Expression<Data_prop>::ptr_t
make_expression<Data_prop>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Data_prop>::ptr_t ptr_t;
   typedef Expression<Data_prop>::Err Err;

   if( ea.handle == owl_topDataProperty::id() ) return ptr_t(new Dp_top());

   if( ea.handle == owl_bottomDataProperty::id() ) return ptr_t(new Dp_bottom());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Dp_declared(ea, ts));

   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported data property")
               << Err::str1_t(ea.string(ts))
   );
}

/*
*******************************************************************************/
template<> Expression<Data_facet>::ptr_t
make_expression<Data_facet>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Data_facet>::ptr_t ptr_t;
   typedef Expression<Data_facet>::Err Err;
   Node_literal const& node = to_literal(ts[ea.obj1]);
   return ptr_t(new Facet_restriction(ea.pred1, node, ts));
}

/*
*******************************************************************************/
template<> Expression<Data_inst>::ptr_t
make_expression<Data_inst>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Data_inst>::ptr_t ptr_t;
   Node_literal const& nl = to_literal(ts[ea.handle]);
   return ptr_t( new D_value(nl, ts));
}

/*
*******************************************************************************/
Expression<Obj_type>::ptr_t
make_restriction_ote(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_type>::ptr_t ptr_t;
   typedef Expression<Obj_type>::Err Err;

   if( ea.pred1 != owl_onProperty::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t(
                     "\"_:x rdf:type owl:Restriction\" without \"_:x "
                     "owl:onProperty y\""
            )
            << Err::str1_t(ea.string(ts))
   );
   const Node_property np = declaration<Node_property>(ea.obj1, ts);
   if( ! np.is_object() && ! np.is_data() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t(
                     "object in \"_:x owl:onProperty y\" should be defined as "
                     "object or data property"
            )
            << Err::str1_t(ea.string(ts))
   );

   switch (ea.cardinality()) {
   case empty_::index: break;

   case owl_cardinality::index:
   case owl_maxCardinality::index:
   case owl_minCardinality::index:
   case owl_maxQualifiedCardinality::index:
   case owl_minQualifiedCardinality::index:
   case owl_qualifiedCardinality::index:
      if( np.is_object() ) return ptr_t(new Ot_opc_restriction(ea, ts));
      if( np.is_data() )   return ptr_t(new Ot_dpc_restriction(ea, ts));
      /* no break */
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unexpected cardinality node in owl:Restriction expression")
            << Err::str1_t(ea.string(ts))
   );
   /* no break */
   }

   switch (ea.pred2()) {
   case owl_allValuesFrom::index:
   case owl_hasValue::index:
      if( np.is_data() )   return ptr_t(new Ot_data_has_value(ea, ts));
      /* no break */
   case owl_hasSelf::index:
   case owl_someValuesFrom::index:
      if( np.is_object() ) return ptr_t(new Ot_op_restriction(ea, ts));
      if( np.is_data() )   return ptr_t(new Ot_dp_restriction(ea, ts));
      /* no break */
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported predicate for owl:Restriction expression")
            << Err::str1_t(ea.string(ts))
   );
   /* no break */
   }
}

/*
*******************************************************************************/
Expression<Obj_type>::ptr_t
make_class_ote(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_type>::ptr_t ptr_t;
   typedef Expression<Obj_type>::Err Err;

   switch (ea.pred1()) {

   case owl_intersectionOf::index:
   case owl_unionOf::index: return ptr_t(new Ot_type_list(ea, ts));
   case owl_complementOf::index: return ptr_t(new Ot_complement(ea, ts));
   case owl_oneOf::index: return ptr_t(new Ot_instance_list(ea, ts));
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported predicate for owl:Class expression")
            << Err::str1_t(ea.string(ts))
   );
   /* no break */
   }
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
