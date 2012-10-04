/** @file "/owlcpp/lib/rdf/print_node.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/print_node.hpp"

#include "boost/numeric/conversion/cast.hpp"
#include "boost/lexical_cast.hpp"

#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_ns.hpp"


namespace owlcpp {

/*
*******************************************************************************/
std::string to_string(const Node_id nid) {
   return "Node" + boost::lexical_cast<std::string>(nid());
}

/*
*******************************************************************************/
std::string to_string(Node_iri const& node) {
   return to_string(node.ns_id()) + ':' + node.name();
}

/*
*******************************************************************************/
std::string to_string_full(Node_iri const& node, Triple_store const& ts) {
   if( node.name().empty() ) return ts[node.ns_id()];
   return ts[node.ns_id()] + '#' + node.name();
}

/*
std::string to_string_pref(Node_iri const& node, Triple_store const& ts) {

}
*******************************************************************************/

/*
std::string to_string(Node_iri const& node, Triple_store const& ts) {
}
*******************************************************************************/

namespace{
/*
*******************************************************************************/
class To_string : public Visitor_node {
   void visit_impl(Node_iri const& node) {
      str_ = to_string(node.ns_id()) + ':' + node.name();
   }

   void visit_impl(Node_blank const& node) {

   }

   void visit_impl(Node_bool const& node) {

   }

   void visit_impl(Node_int const& node) {

   }

   void visit_impl(Node_unsigned const& node) {

   }

   void visit_impl(Node_double const& node) {

   }

   void visit_impl(Node_string const& node) {

   }

public:
   std::string str_;
};

}//namespace anonymous

/*
*******************************************************************************/
std::string to_string(Node const& node) {
   To_string tstr;
   node.accept(tstr);
   return tstr.str_;
}

}//namespace owlcpp
