/** @file "/owlcpp/include/owlcpp/reasoner/detail/node_property.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_PROPERTY_HPP_
#define NODE_PROPERTY_HPP_
#include "owlcpp/reasoner/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{  namespace detail{

/**@brief 
*******************************************************************************/
class Node_property {
public:
   struct Err : public Reasoner_err {};

   Node_property() : p_(P_None) {}
   bool is_object() const {return p_ == Obj_property;}
   bool is_data() const {return p_ == Data_property;}
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
         set(Obj_property);
         break;

      case T_owl_DatatypeProperty::index:
         set(Data_property);
         break;

      case T_owl_AnnotationProperty::index:
         set(Annotation);
         break;
      }
   }

   bool operator==(Node_property const& nt) const {return p_ == nt.p_;}
   bool operator!=(Node_property const& nt) const {return p_ != nt.p_;}

private:
   enum Property {P_None, Obj_property, Data_property, Annotation};
   Property p_;

   void set(const Property p) {
      if( p_ == P_None ) p_ = p;
      if( p_ != p ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("conflicting node property declaration")
         );
      }
   }
};


}//namespace detail
}//namespace owlcpp
#endif /* NODE_PROPERTY_HPP_ */
