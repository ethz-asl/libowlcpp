/** @file "/owlcpp/lib/logic/expressions.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/

#include "expressions.hpp"
#include "boost/foreach.hpp"

namespace owlcpp{ namespace detail{
using namespace owlcpp::terms;

/*
Obj_type::Obj_type(const Node_id handle, Triple_store const& ts)
: ts_(ts),
  handle_(handle),
  type_(T_empty_::id()),
  op_(T_empty_::id())
{
   if( ts_[handle].ns_id() == N_blank::id() ) parse_expression();
}
*******************************************************************************/

/*
TDLConceptExpression* Obj_type::operator()(ReasoningKernel& k) const {
   if( ts_[handle_].ns_id() == N_blank::id() ) return generate_expression(k);
   return k.getExpressionManager()->Concept(ts_.string(handle_));
}
*******************************************************************************/


/*
TDLConceptExpression* Obj_type::generate_expression(ReasoningKernel& k) const {

}
*******************************************************************************/

/*
void Obj_type::parse_class() {
   BOOST_FOREACH(Triple const& t, ts_.triples().find(handle_, any(), any(), any())) {
      const Node_id obj = t.object();
      switch (t.predicate()()) {
         case T_rdf_type::index: continue;

         case T_owl_complementOf::index:
            tv_.push_back(Obj_type(obj, ts_));
            op_ = t.predicate();
            break;
         case T_owl_intersectionOf::index:
         case T_owl_unionOf::index:
         case T_owl_oneOf::index:
            op_ = t.predicate();

            break;
         default:
            break;
      }
   }
}
*******************************************************************************/

/*
void Obj_type::parse_restriction() {

}
*******************************************************************************/
/*
void Obj_type::parse_expression() {
   Query<1,1,0,0>::range r =
            ts_.triples().find(handle_, T_rdf_type::id(), any(), any());
   if( ! r ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("class expression rdf:type not found")
            << Err::str1_t(ts_.string(handle_))
   );
   type_ = r.front().object();
   if( type_ == T_owl_Class::id() ) parse_class();
   else if( type_ == T_owl_Restriction::id() ) parse_restriction();
   else BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported rdf:type")
            << Err::str1_t(ts_.string(type_))
   );
}
*******************************************************************************/

/*
*******************************************************************************/
Obj_type::ptr_t make_class_type(
         const Node_id h,
         Triple_store const& ts
) {

}

/*
*******************************************************************************/
Obj_type::ptr_t make_restriction_type(
         const Node_id h,
         Triple_store const& ts
) {

}
/*
*******************************************************************************/
Obj_type::ptr_t make_obj_type(const Node_id h, Triple_store const& ts) {
   Query<1,1,0,0>::range r =
            ts.triples().find(h, T_rdf_type::id(), any(), any());
   if( ! r ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("class expression rdf:type not found")
            << Logic_err::str1_t(ts.string(h))
   );
   const Node_id t = r.front().object();
   if( t == T_owl_Class::id() ) return make_class_type(h, ts);
   if( t == T_owl_Restriction::id() ) return make_restriction_type(h, ts);
   BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("unsupported rdf:type")
            << Logic_err::str1_t(ts.string(t))
   );

}


}//namespace detail
}//namespace owlcpp
