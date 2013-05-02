/** @file "/owlcpp/include/owlcpp/detail/datatype_impl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DATATYPE_IMPL_HPP_
#define DATATYPE_IMPL_HPP_
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace detail{

enum Internal_type_id {
   Top_tid,
   Empty_tid,
   Bool_tid,
   Int_tid,
   Unsigned_tid,
   Double_tid,
   String_tid,
   Time_tid,
   Unknown_tid
};

}//namespace detail

/**@brief 
*******************************************************************************/
inline detail::Internal_type_id internal_type_id(const Node_id nid) {
   using namespace owlcpp::terms;
   using namespace detail;
   switch (nid()) {
   case empty_::index:
      return Empty_tid;

   case rdfs_Literal::index:
      return Top_tid;
   case xsd_string::index:
   case xsd_anyURI::index:
   case xsd_normalizedString::index:
   case xsd_token::index:
   case xsd_language::index:
   case xsd_NMTOKEN::index:
   case xsd_Name::index:
   case xsd_NCName::index:
   case owl_rational::index:
      return String_tid;

   case xsd_dateTime::index:
   case xsd_time::index:
   case xsd_date::index:
   case xsd_gYearMonth::index:
   case xsd_gYear::index:
   case xsd_gMonthDay::index:
   case xsd_gDay::index:
   case xsd_gMonth::index:
      return Time_tid;

   case xsd_boolean::index:
      return Bool_tid;

   case xsd_decimal::index:
   case xsd_float::index:
   case xsd_double::index:
   case owl_real::index:
      return Double_tid;

   case xsd_hexBinary::index:
   case xsd_base64Binary::index:
   case xsd_integer::index:
   case xsd_nonPositiveInteger::index:
   case xsd_negativeInteger::index:
   case xsd_long::index:
   case xsd_int::index:
   case xsd_short::index:
      return Int_tid;

   case xsd_byte::index:
   case xsd_nonNegativeInteger::index:
   case xsd_unsignedLong::index:
   case xsd_unsignedInt::index:
   case xsd_unsignedShort::index:
   case xsd_unsignedByte::index:
   case xsd_positiveInteger::index:
      return Unsigned_tid;

   default:
      return Unknown_tid;
   }
}

}//namespace owlcpp
#endif /* DATATYPE_IMPL_HPP_ */
