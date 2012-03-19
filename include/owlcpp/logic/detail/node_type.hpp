/** @file "/owlcpp/include/owlcpp/logic/detail/node_type.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TYPE_HPP_
#define NODE_TYPE_HPP_
#include<string>
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{  namespace logic{

/**@brief 
*******************************************************************************/
class Node_type {
   enum Type {None, Object, Data};
   Node_type(const Type t) : c_(t) {}
public:
   struct Err : public Logic_err {};

   static std::string name() {return "type";}
   static Node_type object() {return Node_type(Object);}
   static Node_type data() {return Node_type(Data);}

   Node_type() : c_(None) {}
   bool is_none() const {return c_ == None;}
   bool is_object() const {return c_ == Object;}
   bool is_data() const {return c_ == Data;}

   void set(const Node_id nid) {
      using namespace owlcpp::terms;
      switch (nid()) {
      case T_owl_Class::index:
      case T_owl_Restriction::index:
         set(Object);
         break;

      case T_owl_DataRange::index:
         set(Data);
         break;
      }
   }

   bool operator==(Node_type const& nt) const { return c_ == nt.c_; }
   bool operator!=(Node_type const& nt) const { return c_ != nt.c_; }

   std::string to_string() const {
      switch (c_) {
      case None:     return "undefined";
      case Object:   return "object type";
      case Data:     return "data type";
      default:       return "";
      }
   }

private:
   Type c_;

   void set(const Type c) {
      if( c_ == None ) c_ = c;
      if( c_ != c ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("duplicate node class declaration")
         );
      }
   }

};


}//namespace logic
}//namespace owlcpp
#endif /* NODE_TYPE_HPP_ */
