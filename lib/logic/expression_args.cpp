/** @file "/owlcpp/lib/logic/expression_args.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include "owlcpp/config.hpp"
#include "expression_args.hpp"

#include "boost/foreach.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/query_node.hpp"

namespace owlcpp { namespace logic{
using namespace owlcpp::terms;
/*
*******************************************************************************/
Expression_args::Expression_args(const Node_id h, Triple_store const& ts)
: handle(h),
  e_type    (empty_::id()),
  pred1     (empty_::id()),
  obj1      (empty_::id()),
  pred2     (empty_::id()),
  obj2      (empty_::id()),
  cardinality (empty_::id()),
  num(0)
{
   if( ! is_blank(ts[handle].ns_id()) ) return;

   BOOST_FOREACH(Triple const& t, ts.find_triple(h, any(), any(), any())) {
      const Node_id pred = t.predicate();
      const Node_id obj = t.object();
      switch (pred()) {
         case rdf_type::index:
            e_type = obj;
            break;
         case owl_inverseOf::index:
         case owl_intersectionOf::index:
         case owl_unionOf::index:
         case owl_complementOf::index:
         case owl_oneOf::index:
         case owl_onProperty::index:
            pred1 = pred;
            obj1 = obj;
            break;
         case owl_allValuesFrom::index:
         case owl_hasValue::index:
         case owl_hasSelf::index:
         case owl_someValuesFrom::index:
         case owl_onClass::index:
            pred2 = pred;
            obj2 = obj;
            break;
         case owl_cardinality::index:
         case owl_maxCardinality::index:
         case owl_minCardinality::index:
         case owl_maxQualifiedCardinality::index:
         case owl_minQualifiedCardinality::index:
         case owl_qualifiedCardinality::index:
            cardinality = pred;
            num = value<unsigned>(obj, ts);
            break;
         default:
            BOOST_THROW_EXCEPTION(
                        Err()
                        << Err::msg_t("unsupported class declaration")
                        << Err::str1_t(to_string(h, ts))
                        << Err::str2_t(to_string(pred, ts))
               );
      }
   }
   if( is_empty(e_type) ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("class expression rdf:type not found")
            << Err::str1_t(to_string(h, ts))
   );
   if( e_type != owl_Class::id() && e_type != owl_Restriction::id() )
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported rdf:type")
               << Err::str1_t(to_string(e_type, ts))
   );
}

}//namespace logic
}//namespace owlcpp
