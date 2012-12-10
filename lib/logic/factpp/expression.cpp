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
#include "owlcpp/rdf/print_node.hpp"
#include "logic/node_property_declaration.hpp"
#include "logic/node_type_declaration.hpp"
#include "logic/find_node_declaration.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
template<> Expression<Obj_type>::ptr_t
make_expression<Obj_type>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_type>::ptr_t ptr_t;
   typedef Expression<Obj_type>::Err Err;

   if( ea.handle == T_owl_Thing::id() ) return ptr_t(new Ot_thing());

   if( ea.handle == T_owl_Nothing::id() ) return ptr_t(new Ot_nothing());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Ot_declared(ea, ts));

   if( ea.e_type == T_owl_Restriction::id() ) return make_restriction_ote(ea, ts);

   if( ea.e_type == T_owl_Class::id() ) return make_class_ote(ea, ts);

   switch (ea.pred1()) {

   case T_owl_intersectionOf::index:
   case T_owl_unionOf::index: return ptr_t(new Ot_type_list(ea, ts));

   case T_owl_complementOf::index: return ptr_t(new Ot_complement(ea, ts));

   case T_owl_oneOf::index: return ptr_t(new Ot_instance_list(ea, ts));

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported object type")
            << Err::str1_t(to_string(ea.handle, ts))
   );
   }
}

/*
*******************************************************************************/
template<> Expression<Obj_prop>::ptr_t
make_expression<Obj_prop>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_prop>::ptr_t ptr_t;
   typedef Expression<Obj_prop>::Err Err;

   if( ea.handle == T_owl_topObjectProperty::id() ) return ptr_t(new Op_top());

   if( ea.handle == T_owl_bottomObjectProperty::id() ) return ptr_t(new Op_bottom());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Op_declared(ea, ts));

   if( ea.pred1 == T_owl_inverseOf::id() ) return ptr_t(new Op_inverse(ea, ts));

   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported object property")
               << Err::str1_t(to_string(ea.handle, ts))
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

   if( ea.handle == T_rdfs_Literal::id() || ts[ea.handle].ns_id() == N_xsd::id() )
      return ptr_t(new Dt_standard(ea, ts));

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Dt_declared(ea, ts));

   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported data type")
               << Err::str1_t(to_string(ea.handle, ts))
   );
}

/*
*******************************************************************************/
template<> Expression<Data_prop>::ptr_t
make_expression<Data_prop>(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Data_prop>::ptr_t ptr_t;
   typedef Expression<Data_prop>::Err Err;

   if( ea.handle == T_owl_topDataProperty::id() ) return ptr_t(new Dp_top());

   if( ea.handle == T_owl_bottomDataProperty::id() ) return ptr_t(new Dp_bottom());

   if( is_iri(ts[ea.handle].ns_id()) ) return ptr_t(new Dp_declared(ea, ts));

   BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported data property")
               << Err::str1_t(to_string(ea.handle, ts))
   );
}

/*
*******************************************************************************/
Expression<Obj_type>::ptr_t
make_restriction_ote(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_type>::ptr_t ptr_t;
   typedef Expression<Obj_type>::Err Err;

   if( ea.pred1 != T_owl_onProperty::id() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t(
                     "\"_:x rdf:type owl:Restriction\" without \"_:x "
                     "owl:onProperty y\""
            )
            << Err::str1_t(to_string(ea.handle, ts))
   );
   const Node_property np = declaration<Node_property>(ea.obj1, ts);
   if( ! np.is_object() && ! np.is_data() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t(
                     "object in \"_:x owl:onProperty y\" should be defined as "
                     "object or data property"
            )
            << Err::str1_t(to_string(ea.obj1, ts))
   );

   switch (ea.cardinality()) {
   case T_empty_::index: break;

   case T_owl_cardinality::index:
   case T_owl_maxCardinality::index:
   case T_owl_minCardinality::index:
   case T_owl_maxQualifiedCardinality::index:
   case T_owl_minQualifiedCardinality::index:
   case T_owl_qualifiedCardinality::index:
      if( np.is_object() ) return ptr_t(new Ot_opc_restriction(ea, ts));
      if( np.is_data() )   return ptr_t(new Ot_dpc_restriction(ea, ts));

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unexpected cardinality node in owl:Restriction expression")
            << Err::str1_t(to_string(ea.cardinality, ts))
   );
   }

   switch (ea.pred2()) {
   case T_owl_allValuesFrom::index:
   case T_owl_hasValue::index:
   case T_owl_hasSelf::index:
   case T_owl_someValuesFrom::index:
      if( np.is_object() ) return ptr_t(new Ot_op_restriction(ea, ts));
      if( np.is_data() )   return ptr_t(new Ot_dp_restriction(ea, ts));

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported predicate for owl:Restriction expression")
            << Err::str1_t(to_string(ea.pred1, ts))
   );
   }
}

/*
*******************************************************************************/
Expression<Obj_type>::ptr_t
make_class_ote(Expression_args const& ea, Triple_store const& ts) {
   typedef Expression<Obj_type>::ptr_t ptr_t;
   typedef Expression<Obj_type>::Err Err;

   switch (ea.pred1()) {

   case T_owl_intersectionOf::index:
   case T_owl_unionOf::index: return ptr_t(new Ot_type_list(ea, ts));

   case T_owl_complementOf::index: return ptr_t(new Ot_complement(ea, ts));

   case T_owl_oneOf::index: return ptr_t(new Ot_instance_list(ea, ts));

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported predicate for owl:Class expression")
            << Err::str1_t(to_string(ea.pred1, ts))
   );
   }
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
