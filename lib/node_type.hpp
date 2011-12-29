/** @file "/owl_cpp/lib/node_type.hpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#ifndef NODE_TYPE_HPP_
#define NODE_TYPE_HPP_
#include <string>
#include <vector>
#include <cassert>
#include "loki/Visitor.h"
namespace L = Loki;
#include "owl_cpp/ns_id.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

#include "owl_cpp/node_base.hpp"
#include "owl_cpp/rdf/triple.hpp"

namespace owl_cpp{

/** Literal description term of an RDF triple
*******************************************************************************/
class Node_literal : public Node_base {
public:
   typedef ot::N_empty ns_type;

   Node_literal(const std::string& val) : val_(val) {}

   LOKI_DEFINE_CONST_VISITABLE()

   const std::string& value_str() const {return val_;}

   Ns_id ns_id() const {return ns_type::id();}

   std::auto_ptr<Triple> make_triple(
         const Node_id, const Node_id, const Node_id
   ) const {
      BOOST_THROW_EXCEPTION(
            exception() <<
            exception::msg_t("cannot create triple with Literal predicate")
      );
      return std::auto_ptr<Triple>();
   }

private:
   std::string val_;
};

/** Standard OWL term
*******************************************************************************/
template<class S> struct Node_std : public Node_base {
   typedef S tag_t;

   Ns_id ns_id() const {
      typedef typename tag_t::ns_type ns_type;
      return ns_type::id();
   }

   const std::string& value_str() const {return tag_t::name();}

   LOKI_DEFINE_CONST_VISITABLE()

   std::auto_ptr<Triple> make_triple(
         const Node_id obj, const Node_id pred, const Node_id subj
   ) const {
      return std::auto_ptr<Triple>( new Triple_std<tag_t>(obj, pred, subj) );
   }
};

/** specialize for a generic term
*******************************************************************************/
template<> class Node_std<ot::T_empty_generic> : public Node_base {
public:
   typedef ot::T_empty_generic tag_t;
   Node_std() : ns_( tag_t::ns_type::id() ), name_(tag_t::name()) {}

   Node_std(const Ns_id ns, const std::string& name)
   : ns_(ns), name_(name) {}

   Ns_id ns_id() const {return ns_;}

   const std::string& value_str() const {return name_;}

   LOKI_DEFINE_CONST_VISITABLE()

   std::auto_ptr<Triple> make_triple(
         const Node_id obj, const Node_id pred, const Node_id subj
   ) const {
      return std::auto_ptr<Triple>( new Triple_std<tag_t>(obj, pred, subj) );
   }

private:
   Ns_id ns_;
   std::string name_;
};

typedef Node_std<ot::T_empty_generic> node_generic_t;


/**
Blank node (anonymous resource, or bnode), a node not identified by a URI;
can be used as subject or object in RDF triple.
Blank nodes are either sequences or classes
*******************************************************************************/
class Node_blank : public Node_base {
public:
   typedef ot::N_blank ns_type;
   Node_blank(const std::string& name) : name_(name) {}
   Ns_id ns_id() const {return ns_type::id();}
   const std::string& value_str() const {return name_;}

   std::auto_ptr<Triple> make_triple(
         const Node_id, const Node_id, const Node_id
   ) const {
      BOOST_THROW_EXCEPTION(
            exception() <<
            exception::msg_t("cannot create triple with blank node predicate")
      );
      return std::auto_ptr<Triple>();
   }

   LOKI_DEFINE_CONST_VISITABLE()

private:
   std::string name_;
};

/**
*******************************************************************************/

}//namespace owl_cpp
#endif /* NODE_TYPE_HPP_ */
