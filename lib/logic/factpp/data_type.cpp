/** @file "/owlcpp/lib/logic/factpp/data_type.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "data_type.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
Dt_standard::Dt_standard(Expression_args const& ea, Triple_store const& ts)
   : nid_(ea.handle) {
      switch (nid_()) {
      case empty_::index:
      case xsd_string::index:
      case xsd_anyURI::index:
      case xsd_normalizedString::index:
      case xsd_token::index:
      case xsd_language::index:
      case xsd_NMTOKEN::index:
      case xsd_Name::index:
      case xsd_NCName::index:

      case xsd_boolean::index:

      case xsd_decimal::index:
      case xsd_float::index:
      case xsd_double::index:

      case xsd_dateTime::index:
      case xsd_time::index:
      case xsd_date::index:
      case xsd_gYearMonth::index:
      case xsd_gYear::index:
      case xsd_gMonthDay::index:
      case xsd_gDay::index:
      case xsd_gMonth::index:

      case xsd_hexBinary::index:
      case xsd_base64Binary::index:
      case xsd_integer::index:
      case xsd_nonPositiveInteger::index:
      case xsd_negativeInteger::index:
      case xsd_long::index:
      case xsd_int::index:
      case xsd_short::index:
      case xsd_byte::index:
      case xsd_nonNegativeInteger::index:
      case xsd_unsignedLong::index:
      case xsd_unsignedInt::index:
      case xsd_unsignedShort::index:
      case xsd_unsignedByte::index:
      case xsd_positiveInteger::index:
         break;

      default: BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported standard data type")
               << Err::str1_t(to_string(nid_, ts))
      );
      /* no break */
      }
   }

/*
*******************************************************************************/
Dt_standard::generated_t Dt_standard::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   switch (nid_()) {
   case empty_::index:
   case xsd_string::index:
   case xsd_anyURI::index:
   case xsd_normalizedString::index:
   case xsd_token::index:
   case xsd_language::index:
   case xsd_NMTOKEN::index:
   case xsd_Name::index:
   case xsd_NCName::index:
      return em.getStrDataType();

   case xsd_boolean::index:
      return em.getBoolDataType();

   case xsd_decimal::index:
   case xsd_float::index:
   case xsd_double::index:
      return em.getRealDataType();

   case xsd_dateTime::index:
   case xsd_time::index:
   case xsd_date::index:
   case xsd_gYearMonth::index:
   case xsd_gYear::index:
   case xsd_gMonthDay::index:
   case xsd_gDay::index:
   case xsd_gMonth::index:
      return em.getTimeDataType();

   case xsd_hexBinary::index:
   case xsd_base64Binary::index:
   case xsd_integer::index:
   case xsd_nonPositiveInteger::index:
   case xsd_negativeInteger::index:
   case xsd_long::index:
   case xsd_int::index:
   case xsd_short::index:
   case xsd_byte::index:
   case xsd_nonNegativeInteger::index:
   case xsd_unsignedLong::index:
   case xsd_unsignedInt::index:
   case xsd_unsignedShort::index:
   case xsd_unsignedByte::index:
   case xsd_positiveInteger::index:
      return em.getIntDataType();

   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unsupported standard data type")
            << Err::str1_t(to_string(nid_))
   );
   }
}

/*
*******************************************************************************/
Dt_declared::Dt_declared(Expression_args const& ea, Triple_store const& ts)
: iri_(to_string(ea.handle, ts)) {
   BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
}

/*
*******************************************************************************/
Dt_declared::generated_t Dt_declared::get(ReasoningKernel& k ) const {
   return k.getExpressionManager()->DataType(iri_);
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
