/** @file "/owlcpp/lib/logic/expression_args.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#include "expression_args.hpp"
#include <sstream>
#include "boost/foreach.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/rdf/print_node.hpp"

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

   BOOST_FOREACH(Triple const& t, ts.find_triple(h, any, any, any)) {
      const Node_id pred = t.pred_;
      const Node_id obj = t.obj_;
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
         case owl_onDatatype::index:
         case xsd_maxExclusive::index:
         case xsd_maxInclusive::index:
         case xsd_minExclusive::index:
         case xsd_minInclusive::index:
            pred1 = pred;
            obj1 = obj;
            break;
         case owl_allValuesFrom::index:
         case owl_hasValue::index:
         case owl_hasSelf::index:
         case owl_someValuesFrom::index:
         case owl_onClass::index:
         case owl_withRestrictions::index:
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

         //ignore axiom generating predicates
         case owl_disjointWith::index:
         case owl_equivalentClass::index:
         case owl_equivalentProperty::index:
         case owl_propertyDisjointWith::index:
         case rdfs_domain::index:
         case rdfs_range::index:
         case rdfs_subClassOf::index:
         case rdfs_subPropertyOf::index:
            break;

         default:
            BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("unsupported class declaration")
                     << Err::str1_t(to_string(h, ts))
                     << Err::str2_t(to_string(pred, ts))
            );
            /* no break */
      }
   }
}

std::string Expression_args::string(Triple_store const& ts) const {
   std::ostringstream str;
   str
   << to_string(handle, ts) << ' ' << "type:" << to_string(e_type, ts) << ' '
   << "pred1:" << to_string(pred1, ts) << ' ' << to_string(obj1, ts) << ' '
   << "pred2:" << to_string(pred2, ts) << ' ' << to_string(obj2, ts)
   ;
   if( ! is_empty(cardinality) ) str << " cardinality:" << num;
   return str.str();
}

}//namespace logic
}//namespace owlcpp
