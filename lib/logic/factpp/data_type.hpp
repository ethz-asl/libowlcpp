/** @file "/owlcpp/lib/logic/factpp/data_type.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DATA_TYPE_HPP_
#define DATA_TYPE_HPP_
#include "expression.hpp"
#include "factpp/Kernel.hpp"
#include "boost/assert.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;


/**@brief
*******************************************************************************/
class Dt_standard : public Expression<Data_type> {
public:
   Dt_standard(Expression_args const& ea, Triple_store const& ts)
   : nid_(ea.handle) {
      switch (nid_()) {
      case T_empty_::index:
      case T_xsd_string::index:
      case T_xsd_anyURI::index:
      case T_xsd_normalizedString::index:
      case T_xsd_token::index:
      case T_xsd_language::index:
      case T_xsd_NMTOKEN::index:
      case T_xsd_Name::index:
      case T_xsd_NCName::index:

      case T_xsd_boolean::index:

      case T_xsd_decimal::index:
      case T_xsd_float::index:
      case T_xsd_double::index:

      case T_xsd_dateTime::index:
      case T_xsd_time::index:
      case T_xsd_date::index:
      case T_xsd_gYearMonth::index:
      case T_xsd_gYear::index:
      case T_xsd_gMonthDay::index:
      case T_xsd_gDay::index:
      case T_xsd_gMonth::index:

      case T_xsd_hexBinary::index:
      case T_xsd_base64Binary::index:
      case T_xsd_integer::index:
      case T_xsd_nonPositiveInteger::index:
      case T_xsd_negativeInteger::index:
      case T_xsd_long::index:
      case T_xsd_int::index:
      case T_xsd_short::index:
      case T_xsd_byte::index:
      case T_xsd_nonNegativeInteger::index:
      case T_xsd_unsignedLong::index:
      case T_xsd_unsignedInt::index:
      case T_xsd_unsignedShort::index:
      case T_xsd_unsignedByte::index:
      case T_xsd_positiveInteger::index:
         break;

      default: BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported standard data type")
               << Err::str1_t(to_string_short(nid_, ts))
      );
      }
   }

   generated_t get(ReasoningKernel& k ) const {
      TExpressionManager& em = *k.getExpressionManager();
      switch (nid_()) {
      case T_empty_::index:
      case T_xsd_string::index:
      case T_xsd_anyURI::index:
      case T_xsd_normalizedString::index:
      case T_xsd_token::index:
      case T_xsd_language::index:
      case T_xsd_NMTOKEN::index:
      case T_xsd_Name::index:
      case T_xsd_NCName::index:
         return em.getStrDataType();

      case T_xsd_boolean::index:
         return em.getBoolDataType();

      case T_xsd_decimal::index:
      case T_xsd_float::index:
      case T_xsd_double::index:
         return em.getRealDataType();

      case T_xsd_dateTime::index:
      case T_xsd_time::index:
      case T_xsd_date::index:
      case T_xsd_gYearMonth::index:
      case T_xsd_gYear::index:
      case T_xsd_gMonthDay::index:
      case T_xsd_gDay::index:
      case T_xsd_gMonth::index:
         return em.getTimeDataType();

      case T_xsd_hexBinary::index:
      case T_xsd_base64Binary::index:
      case T_xsd_integer::index:
      case T_xsd_nonPositiveInteger::index:
      case T_xsd_negativeInteger::index:
      case T_xsd_long::index:
      case T_xsd_int::index:
      case T_xsd_short::index:
      case T_xsd_byte::index:
      case T_xsd_nonNegativeInteger::index:
      case T_xsd_unsignedLong::index:
      case T_xsd_unsignedInt::index:
      case T_xsd_unsignedShort::index:
      case T_xsd_unsignedByte::index:
      case T_xsd_positiveInteger::index:
         return em.getIntDataType();

      default: BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unsupported standard data type")
               << Err::str1_t(to_string(nid_))
      );
      }
   }

private:
   const Node_id nid_;
};

/**@brief
*******************************************************************************/
class Dt_declared : public Expression<Data_type> {
public:
   Dt_declared(Expression_args const& ea, Triple_store const& ts)
   : iri_(to_string(ea.handle, ts)) {
      BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
   }

   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->DataType(iri_);
   }

private:
   std::string iri_;
};


}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* DATA_TYPE_HPP_ */
