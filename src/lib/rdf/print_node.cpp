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
#include "boost/functional.hpp"
#include "boost/function.hpp"

#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp {

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
class To_string : public Visitor_node {
   typedef boost::function<std::string(Node_iri const&)> iri_printer_t;

   explicit To_string(iri_printer_t const& iri_printer)
   : iri_printer_(iri_printer)
   {}

public:
   static std::string run(iri_printer_t const& iri_printer, Node const& node) {
      To_string ts(iri_printer);
      node.accept(ts);
      return ts.str_;
   }

private:
   iri_printer_t iri_printer_;
   std::string str_;
//   IRIPrint const& iri_print_;
   void visit_impl(Node_iri const& node) {str_ = iri_printer_(node);}
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
   //resolve overload
   return To_string::run(
            static_cast<std::string(*)(Node_iri const&)>(to_string),
            node
   );
}

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
std::string to_string(Node const& node, Triple_store const& ts) {
   //resolve overload
   typedef std::string(*fun_t)(Node_iri const&, Triple_store const&);
   return To_string::run(
            boost::bind2nd(static_cast<fun_t>(to_string), ts),
            node
   );
}

/**@return IRI node string with complete namespace or prefix (if defined)
*******************************************************************************/
std::string to_string(const Node_id nid, Triple_store const& ts) {
   return to_string(ts[nid], ts);
}

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
std::string to_string_pref(Node const& node, Triple_store const& ts) {
   //resolve overload
   typedef std::string(*fun_t)(Node_iri const&, Triple_store const&);
   return To_string::run(
            boost::bind2nd(static_cast<fun_t>(to_string_pref), ts),
            node
   );
}

/**@return IRI node string with namespace prefix, generated, if needed
*******************************************************************************/
std::string to_string_pref(const Node_id nid, Triple_store const& ts) {
   return to_string_pref(ts[nid], ts);
}

/**@return node string with complete namespace
*******************************************************************************/
std::string to_string_full(Node const& node, Triple_store const& ts) {
   //resolve overload
   typedef std::string(*fun_t)(Node_iri const&, Triple_store const&);
   return To_string::run(
            boost::bind2nd(static_cast<fun_t>(to_string_full), ts),
            node
   );
}

/**@return node string with complete namespace
*******************************************************************************/
std::string to_string_full(const Node_id nid, Triple_store const& ts) {
   return to_string_full(ts[nid], ts);
}

}//namespace owlcpp
