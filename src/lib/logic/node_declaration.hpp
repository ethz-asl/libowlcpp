/** @file "/owlcpp/lib/logic/node_declaration.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef LOGIC_NODE_DECLARATION_HPP_
#define LOGIC_NODE_DECLARATION_HPP_
#include "owlcpp/logic/exception.hpp"

namespace owlcpp{ namespace logic{

/**Base class for storing node declarations
*******************************************************************************/
class Node_declaration {
protected:
   enum Refers_to {None, Object, Data, Annotation};
   struct Err : public Logic_err {};

public:
   bool is_none() const {return ref_ == None;}
   bool is_object() const {return ref_ == Object;}
   bool is_data() const {return ref_ == Data;}
   bool is_annotation() const {return ref_ == Annotation;}
   std::string to_string() const {return to_string_impl(ref_);}
   virtual ~Node_declaration() {}

protected:
   Node_declaration(const Refers_to ref) : ref_(ref) {}

   void set(const Refers_to ref) {
      if( ref_ == None ) ref_ = ref;
      else if( ref_ != ref ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t(
                           "conflicting node declaration: defined as " +
                           to_string() + " and as " +
                           to_string_impl(ref)
                  )
      );
      }
   }

   virtual std::string to_string_impl(const Refers_to ref) const =0;

   Refers_to ref_;
};

}//namespace logic
}//namespace owlcpp
#endif /* LOGIC_NODE_DECLARATION_HPP_ */
