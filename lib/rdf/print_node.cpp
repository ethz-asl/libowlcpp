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
#include "owlcpp/rdf/print_id.hpp"


namespace owlcpp {

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

/*
*******************************************************************************/
std::string to_string(Node_iri const& node) {
   return to_string(node.ns_id()) + ':' + node.name();
}

/*
*******************************************************************************/
std::string to_string(Node_blank const& node) {
   return "_:" + to_string(node.document()) + '-' +
            boost::lexical_cast<std::string>(node.index());
}

/*
*******************************************************************************/
std::string to_string(Node_bool const& node) {
   return node.value() ? "true" : "false";
}

/*
*******************************************************************************/
std::string to_string(Node_int const& node) {
   return '"' + boost::lexical_cast<std::string>(node.value()) + '"';
}

/*
*******************************************************************************/
std::string to_string(Node_unsigned const& node) {
   return '"' + boost::lexical_cast<std::string>(node.value()) + '"';
}

/*
*******************************************************************************/
std::string to_string(Node_double const& node) {
   return '"' + boost::lexical_cast<std::string>(node.value()) + '"';
}

/*
*******************************************************************************/
std::string to_string(Node_string const& node) {
   return '"' + node.value() + '"';
}

namespace{
/*
*******************************************************************************/
class To_string : public Visitor_node {
   void visit_impl(Node_iri const& node) {str_ = to_string(node);}
   void visit_impl(Node_blank const& node) {str_ = to_string(node);}
   void visit_impl(Node_bool const& node) {str_ = to_string(node);}
   void visit_impl(Node_int const& node) {str_ = to_string(node);}
   void visit_impl(Node_unsigned const& node) {str_ = to_string(node);}
   void visit_impl(Node_double const& node) {str_ = to_string(node);}
   void visit_impl(Node_string const& node) {str_ = to_string(node);}
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
