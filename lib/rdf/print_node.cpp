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

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
std::string to_string(Node_iri const& node, Triple_store const& ts) {
   const Ns_id nsid = node.ns_id();
   const std::string pref = ts.prefix(nsid);
   if( pref.empty() ) return to_string_full(node, ts);
   return pref + ':' + node.name();
}

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
std::string to_string_pref(Node_iri const& node, Triple_store const& ts) {
   const Ns_id nsid = node.ns_id();
   const std::string pref = ts.prefix(nsid);
   if( pref.empty() ) return to_string(node);
   return pref + ':' + node.name();
}

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
std::string to_string(Node_bool const& node) {return node.value_str();}

/*
*******************************************************************************/
std::string to_string(Node_int const& node) {return node.value_str();}

/*
*******************************************************************************/
std::string to_string(Node_unsigned const& node) {return node.value_str();}

/*
*******************************************************************************/
std::string to_string(Node_double const& node) {return node.value_str();}

/*
*******************************************************************************/
std::string to_string(Node_string const& node) {
   return '"' + node.value() + '"';
}

namespace{
/*
*******************************************************************************/
template<class IRIPrint> class To_string : public Visitor_node {
public:
   To_string(IRIPrint const& iri_print, Node const& node)
   : iri_print_(iri_print)
   {
      node.accept(*this);
   }
   std::string str_;

private:
   IRIPrint const& iri_print_;
   void visit_impl(Node_iri const& node) {str_ = iri_print_(node);}
   void visit_impl(Node_blank const& node) {str_ = to_string(node);}
   void visit_impl(Node_bool const& node) {str_ = to_string(node);}
   void visit_impl(Node_int const& node) {str_ = to_string(node);}
   void visit_impl(Node_unsigned const& node) {str_ = to_string(node);}
   void visit_impl(Node_double const& node) {str_ = to_string(node);}
   void visit_impl(Node_string const& node) {str_ = to_string(node);}
};

}//namespace anonymous

/*
*******************************************************************************/
std::string to_string(Node const& node) {
   To_string<std::string(&)(Node_iri const&)> tstr(to_string, node);
   return tstr.str_;
}

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
std::string to_string(Node const& node, Triple_store const& ts) {
//   To_string_ts tstr(ts, node);
//   return tstr.str_;
   return to_string(node);
}

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
std::string to_string(const Node_id nid, Triple_store const& ts) {
   return to_string(ts[nid], ts);
}

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
//std::string to_string_pref(Node const& node, Triple_store const& ts) {
//
//}

/**@return node string with complete namespace
*******************************************************************************/
//std::string to_string_full(Node const& node, Triple_store const& ts) {
//
//}

}//namespace owlcpp
