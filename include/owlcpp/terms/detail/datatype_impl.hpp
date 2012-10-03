/** @file "/owlcpp/include/owlcpp/terms/detail/datatype_impl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DATATYPE_IMPL_HPP_
#define DATATYPE_IMPL_HPP_
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace terms{

enum Internal_type_id {
   Empty_tid, Bool_tid, Int_tid, Unsigned_tid, Double_tid, String_tid, Unknown_tid
};

/**@brief 
*******************************************************************************/
Internal_type_id internal_type_id(const Node_id nid) {
   switch (dt()) {
   case T_empty_::index:
      return Empty_tid;

   case T_xsd_string::index:
   case T_xsd_anyURI::index:
   case T_xsd_normalizedString::index:
   case T_xsd_token::index:
   case T_xsd_language::index:
   case T_xsd_NMTOKEN::index:
   case T_xsd_Name::index:
   case T_xsd_NCName::index:

   case T_xsd_dateTime::index:
   case T_xsd_time::index:
   case T_xsd_date::index:
   case T_xsd_gYearMonth::index:
   case T_xsd_gYear::index:
   case T_xsd_gMonthDay::index:
   case T_xsd_gDay::index:
   case T_xsd_gMonth::index:
      return String_tid;

   case T_xsd_boolean::index:
      return Bool_tid;

   case T_xsd_decimal::index:
   case T_xsd_float::index:
   case T_xsd_double::index:
      return Double_tid;

   case T_xsd_hexBinary::index:
   case T_xsd_base64Binary::index:
   case T_xsd_integer::index:
   case T_xsd_nonPositiveInteger::index:
   case T_xsd_negativeInteger::index:
   case T_xsd_long::index:
   case T_xsd_int::index:
   case T_xsd_short::index:
      return Int_tid;

   case T_xsd_byte::index:
   case T_xsd_nonNegativeInteger::index:
   case T_xsd_unsignedLong::index:
   case T_xsd_unsignedInt::index:
   case T_xsd_unsignedShort::index:
   case T_xsd_unsignedByte::index:
   case T_xsd_positiveInteger::index:
      return Unsigned_tid;

   default:
      return Unknown_tid;
   }
}

}//namespace terms
}//namespace owlcpp
#endif /* DATATYPE_IMPL_HPP_ */
