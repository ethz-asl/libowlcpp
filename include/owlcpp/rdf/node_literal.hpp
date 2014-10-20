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
#include "owlcpp/rdf/literal_datatypes.hpp"
#include "owlcpp/rdf/node_fwd.hpp"
#include "owlcpp/rdf/visitor_node.hpp"

namespace owlcpp{

/**@brief Abstract literal node class
*******************************************************************************/
class Node_literal : public Node {
public:
   Node_id datatype() const {return datatype_impl();}
   std::string value_str() const {return value_str_impl();}
private:
   virtual Node_id datatype_impl() const =0;
   virtual std::string value_str_impl() const =0;
   Ns_id ns_id_impl() const { return terms::empty::id(); }
};

namespace detail{

/**@brief 
*******************************************************************************/
template<class Dt> class Node_literal_impl : public Node_literal {
   typedef Node_literal_impl self_type;
   typedef typename Dt::default_datatype default_datatype;

public:
   typedef typename Dt::value_type value_type;

   template<class In> static value_type convert(In const& in, const Node_id dt) {
      try{
         return Dt::convert(in, dt);
      } catch(std::bad_cast const&) {
         BOOST_THROW_EXCEPTION(
                  Rdf_err()
                  << Rdf_err::msg_t("conversion error")
                  << Rdf_err::str1_t(
                           "\"" + boost::lexical_cast<std::string>(in) + "\""
                  )
                  << Rdf_err::str2_t(Dt::name_str(dt))
         );
      }
   }

   template<class T> explicit Node_literal_impl(
            T const& val,
            const Node_id dt = default_datatype::id()
   )
   : val_(convert<T>(val, dt)), dt_(dt)
   {}

   explicit Node_literal_impl(
            char const* val,
            const Node_id dt = default_datatype::id()
   )
   : val_(convert(std::string(val), dt)), dt_(dt)
   {}

   explicit Node_literal_impl(
            std::string const& val,
            const Node_id dt = default_datatype::id()
   )
   : val_(convert(val, dt)), dt_(dt)
   {}

   value_type value() const {return val_;}

private:
   value_type val_;
   Node_id dt_;

   OWLCPP_VISITABLE

   std::string value_str_impl() const {return Dt::to_string(val_, dt_);}

   Node_id datatype_impl() const {return dt_;}

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
template<> class Node_literal_impl<Datatype_string> : public Node_literal {
   typedef Node_literal_impl self_type;
   typedef Datatype_string::default_datatype default_datatype;

   static std::size_t _hash(std::string const& val, const Node_id dt, std::string const& lang) {
      std::size_t h = 0;
      boost::hash_combine(h, val);
      boost::hash_combine(h, lang);
      boost::hash_combine(h, dt);
      return h;
   }

public:
   typedef Datatype_string::value_type value_type;

   explicit Node_literal_impl(
            std::string const& val,
            const Node_id dt,
            std::string const& lang = ""
   )
   : val_(val), lang_(lang), hash_(_hash(val, dt, lang)), dt_(dt)
   {}

   explicit Node_literal_impl(
            std::string const& val,
            std::string const& lang = ""
   )
   : val_(val),
     lang_(lang),
     hash_(_hash(val, default_datatype::id(), lang)),
     dt_(default_datatype::id())
   {}

   std::string const& language() const {return lang_;}
   std::string const& value() const {return val_;}

private:
   std::string val_;
   std::string lang_;
   std::size_t hash_;
   Node_id dt_;

   OWLCPP_VISITABLE

   Node_id datatype_impl() const {return dt_;}

   std::string value_str_impl() const {return val_;}

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
