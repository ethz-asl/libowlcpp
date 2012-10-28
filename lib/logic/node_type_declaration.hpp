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
      case T_rdfs_subClassOf::index:
      case T_owl_Class::index:
      case T_owl_Restriction::index:
      case T_owl_Thing::index:
      case T_owl_Nothing::index:
         Node_declaration::set(Object);
         break;

      case T_rdfs_Datatype::index:
      case T_rdfs_Literal::index:
      case T_rdf_PlainLiteral::index:
      case T_rdf_XMLLiteral::index:
      case T_owl_rational::index:
      case T_owl_real::index:
      case T_xsd_ENTITIES::index:
      case T_xsd_ENTITY::index:
      case T_xsd_ID::index:
      case T_xsd_IDREF::index:
      case T_xsd_IDREFS::index:
      case T_xsd_NCName::index:
      case T_xsd_NMTOKEN::index:
      case T_xsd_NOTATION::index:
      case T_xsd_Name::index:
      case T_xsd_QName::index:
      case T_xsd_anyURI::index:
      case T_xsd_base64Binary::index:
      case T_xsd_boolean::index:
      case T_xsd_byte::index:
      case T_xsd_date::index:
      case T_xsd_dateTime::index:
      case T_xsd_dateTimeStamp::index:
      case T_xsd_decimal::index:
      case T_xsd_double::index:
      case T_xsd_duration::index:
      case T_xsd_float::index:
      case T_xsd_gDay::index:
      case T_xsd_gMonth::index:
      case T_xsd_gMonthDay::index:
      case T_xsd_gYear::index:
      case T_xsd_gYearMonth::index:
      case T_xsd_hexBinary::index:
      case T_xsd_int::index:
      case T_xsd_integer::index:
      case T_xsd_language::index:
      case T_xsd_long::index:
      case T_xsd_negativeInteger::index:
      case T_xsd_nonNegativeInteger::index:
      case T_xsd_nonPositiveInteger::index:
      case T_xsd_normalizedString::index:
      case T_xsd_positiveInteger::index:
      case T_xsd_short::index:
      case T_xsd_string::index:
      case T_xsd_time::index:
      case T_xsd_token::index:
      case T_xsd_unsignedByte::index:
      case T_xsd_unsignedInt::index:
      case T_xsd_unsignedLong::index:
      case T_xsd_unsignedShort::index:
         Node_declaration::set(Data);
         break;
      }
   }

   bool operator==(Node_type const& nt) const { return ref_ == nt.ref_; }
   bool operator!=(Node_type const& nt) const { return ref_ != nt.ref_; }

   std::string to_string() const {
      switch (ref_) {
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
