/** @file "/owlcpp/lib/rdf/node_literal.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_literal.hpp"

#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/query_node.hpp"

namespace owlcpp {
using namespace owlcpp::terms;

/**@brief
*******************************************************************************/
std::auto_ptr<Node> make_node_literal(
         std::string const& val,
         const Node_id dt,
         std::string const& lang
) {
   typedef std::auto_ptr<Node> ptr;
   switch (dt()) {
   case T_empty_::index:
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
      return ptr(new Node_string(val, dt, lang));

   case T_xsd_boolean::index:
      if( val == "true" ) return ptr(new Node_bool(true, dt));
      if( val == "false" ) return ptr(new Node_bool(false, dt));
      return ptr(new Node_bool(boost::lexical_cast<bool>(val), dt));

   case T_xsd_decimal::index:
   case T_xsd_float::index:
   case T_xsd_double::index:
      return ptr(new Node_double(boost::lexical_cast<double>(val), dt));

   case T_xsd_hexBinary::index:
   case T_xsd_base64Binary::index:
   case T_xsd_integer::index:
   case T_xsd_nonPositiveInteger::index:
   case T_xsd_negativeInteger::index:
   case T_xsd_long::index:
   case T_xsd_int::index:
   case T_xsd_short::index:
      return ptr(new Node_int(boost::lexical_cast<Node_int::value_type>(val), dt));

   case T_xsd_byte::index:
   case T_xsd_nonNegativeInteger::index:
   case T_xsd_unsignedLong::index:
   case T_xsd_unsignedInt::index:
   case T_xsd_unsignedShort::index:
   case T_xsd_unsignedByte::index:
   case T_xsd_positiveInteger::index:
      return ptr(new Node_unsigned(boost::lexical_cast<Node_unsigned::value_type>(val), dt));

   default: BOOST_THROW_EXCEPTION(
            Rdf_err()
            << Rdf_err::msg_t("unsupported data type")
            << Rdf_err::str1_t(to_string(dt))
   );
   }
}

}//namespace owlcpp
