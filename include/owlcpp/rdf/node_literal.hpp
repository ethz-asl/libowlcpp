/** @file "/owlcpp/include/owlcpp/rdf/node_literal.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_LITERAL_HPP_
#define NODE_LITERAL_HPP_
#include <iostream>
#include "boost/functional/hash.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_literal : public Node_v {
public:
   Node_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id(),
            std::string const& lang = ""
   )
   : val_(val), lang_(lang), dt_(dt)
   {}

   std::string const& language() const {return lang_;}
   Node_id datatype() const {return dt_;}

private:
   std::string val_;
   std::string lang_;
   Node_id dt_;

   //todo
   std::string value_str_impl() const {return val_;}

   //todo
   std::ostream& print_impl(std::ostream& os) const {
      return os << val_;
   }

   Ns_id ns_id_impl() const { return terms::N_empty::id(); }

   bool empty_impl() const { return val_.empty() && lang_.empty() && is_empty(dt_); }

   bool equal_impl(const Node_v& n) const {
      if( Node_literal const*const p = dynamic_cast<Node_literal const*>(&n) ) {
         return dt_ == p->dt_ && lang_ == p->lang_ && val_ == p->val_;
      }
      return false;
   }

   std::size_t hash_impl() const {
      std::size_t x = 0;
      boost::hash_combine(x, dt_());
      boost::hash_combine(x, boost::hash_value(lang_));
      boost::hash_combine(x, boost::hash_value(val_));
      return x;
   }

};

}//namespace owlcpp
#endif /* NODE_LITERAL_HPP_ */
