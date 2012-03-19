/** @file "/owlcpp/include/owlcpp/logic/detail/node_property.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_PROPERTY_HPP_
#define NODE_PROPERTY_HPP_
#include<string>
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{  namespace logic{

/**@brief 
*******************************************************************************/
class Node_property {
   enum Property {None, Object, Data, Annotation};
   Node_property(const Property p) : p_(p) {}
public:
   struct Err : public Logic_err {};

   static std::string name() {return "property";}
   static Node_property object() {return Node_property(Object);}
   static Node_property data() {return Node_property(Data);}
   static Node_property annotation() {return Node_property(Annotation);}

   Node_property() : p_(None) {}
   bool is_none() const {return p_ == None;}
   bool is_object() const {return p_ == Object;}
   bool is_data() const {return p_ == Data;}
   bool is_annotation() const {return p_ == Annotation;}

   void set(const Node_id nid) {
      using namespace owlcpp::terms;
      switch (nid()) {
      case T_owl_AsymmetricProperty::index:
      case T_owl_FunctionalProperty::index:
      case T_owl_InverseFunctionalProperty::index:
      case T_owl_IrreflexiveProperty::index:
      case T_owl_ObjectProperty::index:
      case T_owl_ReflexiveProperty::index:
      case T_owl_SymmetricProperty::index:
      case T_owl_TransitiveProperty::index:
         set(Object);
         break;

      case T_owl_DatatypeProperty::index:
         set(Data);
         break;

      case T_owl_AnnotationProperty::index:
         set(Annotation);
         break;
      }
   }

   bool operator==(Node_property const& nt) const {return p_ == nt.p_;}
   bool operator!=(Node_property const& nt) const {return p_ != nt.p_;}

   std::string to_string() const {
      switch (p_) {
      case None:        return "undefined";
      case Object:      return "object property";
      case Data:        return "data property";
      case Annotation:  return "annotation property";
      default:          return "";
      }
   }

   private:
   Property p_;

   void set(const Property p) {
      if( p_ == None ) p_ = p;
      if( p_ != p ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("conflicting node property declaration")
         );
      }
   }
};


}//namespace logic
}//namespace owlcpp
#endif /* NODE_PROPERTY_HPP_ */
