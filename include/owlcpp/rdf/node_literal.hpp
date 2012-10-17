/** @file "/owlcpp/include/owlcpp/rdf/node_literal.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_LITERAL_HPP_
#define NODE_LITERAL_HPP_
#include <string>
#include "boost/functional/hash.hpp"

#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/rdf/literal_datatypes.hpp"
#include "owlcpp/rdf/node_fwd.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
template<class Dt> class Node_literal : public Node {
   typedef Node_literal self_type;

public:
   typedef typename Dt::value_type value_type;

   template<class In> static value_type convert(In const& in, const Node_id dt) {
      try{
         return Dt::convert(in, dt);
      } catch(std::bad_cast const&) {
         BOOST_THROW_EXCEPTION(
                  Rdf_err()
                  << Rdf_err::msg_t(
                           std::string("error converting to ") + Dt::name_str(dt)
                  )
                  << Rdf_err::str1_t(boost::lexical_cast<std::string>(in))
         );
      }
   }

   template<class T> explicit Node_literal(
            T const& val,
            const Node_id dt = terms::T_empty_::id()
   )
   : val_(convert<T>(val, dt)), dt_(dt)
   {}

   explicit Node_literal(
            char const* val,
            const Node_id dt = terms::T_empty_::id()
   )
   : val_(convert(std::string(val), dt)), dt_(dt)
   {}

   explicit Node_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id()
   )
   : val_(convert(val, dt)), dt_(dt)
   {}

   Node_id datatype() const {return dt_;}
   value_type value() const {return val_;}
   std::string value_str() const {return Dt::to_string(val_, dt_);}

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

   Node* clone_impl() const {return new self_type(*this);}

};

/**@brief
*******************************************************************************/
template<> class Node_literal<Datatype_string> : public Node {
   typedef Node_literal self_type;

   static std::size_t _hash(std::string const& val, const Node_id dt, std::string const& lang) {
      std::size_t h = 0;
      boost::hash_combine(h, val);
      boost::hash_combine(h, lang);
      boost::hash_combine(h, dt);
      return h;
   }

public:
   typedef Datatype_string::value_type value_type;

   explicit Node_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id(),
            std::string const& lang = ""
   )
   : val_(val), lang_(lang), hash_(_hash(val, dt, lang)), dt_(dt)
   {}

   std::string const& language() const {return lang_;}
   Node_id datatype() const {return dt_;}
   std::string const& value() const {return val_;}

private:
   std::string val_;
   std::string lang_;
   std::size_t hash_;
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

   std::size_t hash_impl() const {return hash_;}

   Node* clone_impl() const {return new self_type(*this);}
};

}//namespace detail
}//namespace owlcpp
#endif /* NODE_LITERAL_HPP_ */
