/** @file "/owlcpp/include/owlcpp/reasoner/detail/node_type.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TYPE_HPP_
#define NODE_TYPE_HPP_
#include "owlcpp/reasoner/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{  namespace detail{

/**@brief 
*******************************************************************************/
class Node_type {
public:
   struct Err : public Reasoner_err {};

   Node_type() : c_(C_None), p_(P_None) {}
   bool is_class() const {return c_ == Class;}
   bool is_data_range() const {return c_ == Data_Range;}
   bool is_obj_property() const {return p_ == Obj_property;}
   bool is_data_property() const {return p_ == Data_property;}
   bool is_annotation_property() const {return p_ == Annotation;}

   void set(const Node_id nid) {
      using namespace owlcpp::terms;
      switch (nid()) {
      case T_owl_Class::index:
         set(Node_type::Class);
         break;

      case T_owl_DataRange::index:
         set(Node_type::Data_Range);
         break;

      case T_owl_AsymmetricProperty::index:
      case T_owl_FunctionalProperty::index:
      case T_owl_InverseFunctionalProperty::index:
      case T_owl_IrreflexiveProperty::index:
      case T_owl_ObjectProperty::index:
      case T_owl_ReflexiveProperty::index:
      case T_owl_SymmetricProperty::index:
      case T_owl_TransitiveProperty::index:
         set(Node_type::Obj_property);
         break;

      case T_owl_DatatypeProperty::index:
         set(Node_type::Data_property);
         break;

      case T_owl_AnnotationProperty::index:
         set(Node_type::Annotation);
         break;
      }
   }

private:
   enum Type {C_None, Class, Data_Range};
   enum Property {P_None, Obj_property, Data_property, Annotation};
   Type c_;
   Property p_;

   void set(const Type c) {
      if( c_ == C_None ) c_ = c;
      if( c_ != c ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("duplicate node class declaration")
         );
      }
   }

   void set(const Property p) {
      if( p_ == P_None ) p_ = p;
      if( p_ != p ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("duplicate node property declaration")
         );
      }
   }
};


}//namespace detail
}//namespace owlcpp
#endif /* NODE_TYPE_HPP_ */
