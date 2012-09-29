/** @file "/owlcpp/lib/logic/expression_args.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include "owlcpp/config.hpp"
#include "expression_args.hpp"

#include "boost/foreach.hpp"

#include "owlcpp/rdf/store_triple.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp { namespace logic{
using namespace owlcpp::terms;
/*
*******************************************************************************/
Expression_args::Expression_args(const Node_id h, Triple_store const& ts)
: handle(h),
  e_type    (T_empty_::id()),
  pred1     (T_empty_::id()),
  obj1      (T_empty_::id()),
  pred2     (T_empty_::id()),
  obj2      (T_empty_::id()),
  cardinality (T_empty_::id()),
  num(0)
{
   if( ! is_blank(ts[handle].ns_id()) ) return;

   BOOST_FOREACH(Triple const& t, ts.triples().find(h, any(), any(), any())) {
      const Node_id pred = t.predicate();
      const Node_id obj = t.object();
      switch (pred()) {
         case T_rdf_type::index:
            e_type = obj;
            break;
         case T_owl_inverseOf::index:
         case T_owl_intersectionOf::index:
         case T_owl_unionOf::index:
         case T_owl_complementOf::index:
         case T_owl_oneOf::index:
         case T_owl_onProperty::index:
            pred1 = pred;
            obj1 = obj;
            break;
         case T_owl_allValuesFrom::index:
         case T_owl_hasValue::index:
         case T_owl_hasSelf::index:
         case T_owl_someValuesFrom::index:
         case T_owl_onClass::index:
            pred2 = pred;
            obj2 = obj;
            break;
         case T_owl_cardinality::index:
         case T_owl_maxCardinality::index:
         case T_owl_minCardinality::index:
         case T_owl_maxQualifiedCardinality::index:
         case T_owl_minQualifiedCardinality::index:
         case T_owl_qualifiedCardinality::index:
            cardinality = pred;
            num = value<unsigned>(obj, ts);
            break;
         default:
            BOOST_THROW_EXCEPTION(
                        Err()
                        << Err::msg_t("unsupported class declaration")
                        << Err::str1_t(to_string_short(h, ts))
                        << Err::str2_t(to_string_short(pred, ts))
               );
      }
   }
   if( is_empty(e_type) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("class expression rdf:type not found")
            << Err::str1_t(to_string_short(h, ts))
   );
   if( e_type != T_owl_Class::id() && e_type != T_owl_Restriction::id() )
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported rdf:type")
               << Err::str1_t(to_string_short(e_type, ts))
   );
}

}//namespace logic
}//namespace owlcpp
