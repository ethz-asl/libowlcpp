/** @file "/owlcpp/include/owlcpp/rdf/node_literal.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_LITERAL_HPP_
#define NODE_LITERAL_HPP_
#include <string>
#include "boost/functional/hash.hpp"
#include "boost/lexical_cast.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{ namespace detail{

template<class Out> struct Convert {
   static Out conv(std::string const& in) {
      return boost::lexical_cast<Out>(in);
   }
};

template<> struct Convert<bool> {
   static bool conv(std::string const& in) {
      if( in == "true" ) return true;
      if( in == "false" ) return false;
      return boost::lexical_cast<bool>(in);
   }
};

/**@brief 
*******************************************************************************/
template<class T> class Node_literal : public Node {
   typedef Node_literal self_type;
public:
   typedef T value_type;

   explicit Node_literal(
            const value_type val,
            const Node_id dt = terms::T_empty_::id()
   )
   : val_(val), dt_(dt)
   {}

   explicit Node_literal(
            char const* val,
            const Node_id dt = terms::T_empty_::id()
   )
   : val_(Convert<value_type>::conv(val)), dt_(dt)
   {}

   explicit Node_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id()
   )
   : val_(Convert<value_type>::conv(val)), dt_(dt)
   {}

   Node_id datatype() const {return dt_;}
   value_type value() const {return val_;}

private:
   value_type val_;
   Node_id dt_;

   OWLCPP_VISITABLE

   Ns_id ns_id_impl() const { return terms::N_empty::id(); }

   bool empty_impl() const { return false; }

   bool equal_impl(const Node& n) const {
      //todo: use typeid()?
      if( self_type const*const p = dynamic_cast<self_type const*>(&n) ) {
         return dt_ == p->dt_ && val_ == p->val_;
      }
      return false;
   }

   std::size_t hash_impl() const {
      std::size_t h = 0;
      boost::hash_combine(h, val_);
      boost::hash_combine(h, dt_);
      return h;
   }

};

/**@brief
*******************************************************************************/
template<> class Node_literal<std::string> : public Node {
   typedef Node_literal self_type;
public:
   typedef std::string value_type;

   explicit Node_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id(),
            std::string const& lang = ""
   )
   : val_(val), lang_(lang), dt_(dt)
   {}

   std::string const& language() const {return lang_;}
   Node_id datatype() const {return dt_;}
   std::string const& value() const {return val_;}

private:
   std::string val_;
   std::string lang_;
   Node_id dt_;

   OWLCPP_VISITABLE

   Ns_id ns_id_impl() const { return terms::N_empty::id(); }

   bool empty_impl() const { return val_.empty() && lang_.empty() && is_empty(dt_); }

   bool equal_impl(const Node& n) const {
      //todo: use typeid()?
      if( self_type const*const p = dynamic_cast<self_type const*>(&n) ) {
         return dt_ == p->dt_ && lang_ == p->lang_ && val_ == p->val_;
      }
      return false;
   }

   std::size_t hash_impl() const {
      std::size_t h = 0;
      boost::hash_combine(h, val_);
      boost::hash_combine(h, lang_);
      boost::hash_combine(h, dt_);
      return h;
   }
};

}//namespace detail
}//namespace owlcpp
#endif /* NODE_LITERAL_HPP_ */
