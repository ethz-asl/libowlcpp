/** @file "/owlcpp/include/owlcpp/logic/detail/node_type.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TYPE_HPP_
#define NODE_TYPE_HPP_
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{  namespace detail{

/**@brief 
*******************************************************************************/
class Node_type {
public:
   struct Err : public Logic_err {};

   Node_type() : c_(C_None) {}
   bool is_class() const {return c_ == Class;}
   bool is_data() const {return c_ == Data_Range;}

   void set(const Node_id nid) {
      using namespace owlcpp::terms;
      switch (nid()) {
      case T_owl_Class::index:
      case T_owl_Restriction::index:
         set(Class);
         break;

      case T_owl_DataRange::index:
         set(Data_Range);
         break;
      }
   }

   bool operator==(Node_type const& nt) const { return c_ == nt.c_; }
   bool operator!=(Node_type const& nt) const { return c_ != nt.c_; }

private:
   enum Type {C_None, Class, Data_Range};
   Type c_;

   void set(const Type c) {
      if( c_ == C_None ) c_ = c;
      if( c_ != c ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("duplicate node class declaration")
         );
      }
   }

};


}//namespace detail
}//namespace owlcpp
#endif /* NODE_TYPE_HPP_ */
