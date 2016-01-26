/** @file "/owlcpp/lib/logic/node_type_declaration.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef LOGIC_NODE_TYPE_DECLARATION_HPP_
#define LOGIC_NODE_TYPE_DECLARATION_HPP_
#include<string>
#include "logic/node_declaration.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace logic{

/**Store whether node is declared as object type or data type
*******************************************************************************/
class Node_type : public Node_declaration {

   Node_type(const Refers_to ref) : Node_declaration(ref) {}

public:
   static std::string name() {return "type";}
   static Node_type object() {return Node_type(Object);}
   static Node_type data() {return Node_type(Data);}

   Node_type() : Node_declaration(None) {}

   void set(const Node_id nid) {
      using namespace owlcpp::terms;
      switch (nid()) {
      case rdfs_subClassOf::index:
      case owl_Class::index:
      case owl_Restriction::index:
      case owl_Thing::index:
      case owl_Nothing::index:
         Node_declaration::set(Object);
         break;

      case rdfs_Datatype::index:
      case rdfs_Literal::index:
      case rdf_PlainLiteral::index:
      case rdf_XMLLiteral::index:
      case owl_rational::index:
      case owl_real::index:
      case xsd_ENTITIES::index:
      case xsd_ENTITY::index:
      case xsd_ID::index:
      case xsd_IDREF::index:
      case xsd_IDREFS::index:
      case xsd_NCName::index:
      case xsd_NMTOKEN::index:
      case xsd_NOTATION::index:
      case xsd_Name::index:
      case xsd_QName::index:
      case xsd_anyURI::index:
      case xsd_base64Binary::index:
      case xsd_boolean::index:
      case xsd_byte::index:
      case xsd_date::index:
      case xsd_dateTime::index:
      case xsd_dateTimeStamp::index:
      case xsd_decimal::index:
      case xsd_double::index:
      case xsd_duration::index:
      case xsd_float::index:
      case xsd_gDay::index:
      case xsd_gMonth::index:
      case xsd_gMonthDay::index:
      case xsd_gYear::index:
      case xsd_gYearMonth::index:
      case xsd_hexBinary::index:
      case xsd_int::index:
      case xsd_integer::index:
      case xsd_language::index:
      case xsd_long::index:
      case xsd_negativeInteger::index:
      case xsd_nonNegativeInteger::index:
      case xsd_nonPositiveInteger::index:
      case xsd_normalizedString::index:
      case xsd_positiveInteger::index:
      case xsd_short::index:
      case xsd_string::index:
      case xsd_time::index:
      case xsd_token::index:
      case xsd_unsignedByte::index:
      case xsd_unsignedInt::index:
      case xsd_unsignedLong::index:
      case xsd_unsignedShort::index:
         Node_declaration::set(Data);
         break;
      }
   }

   bool operator==(Node_type const& nt) const { return ref_ == nt.ref_; }
   bool operator!=(Node_type const& nt) const { return ref_ != nt.ref_; }

private:
   std::string to_string_impl(const Refers_to ref) const {
      switch (ref) {
      case None:     return "undefined";
      case Object:   return "object type";
      case Data:     return "data type";
      default:
         BOOST_THROW_EXCEPTION(
                  Logic_err() << Logic_err::msg_t("node type error")
         );
      }
   }

};


}//namespace logic
}//namespace owlcpp
#endif /* LOGIC_NODE_TYPE_DECLARATION_HPP_ */
