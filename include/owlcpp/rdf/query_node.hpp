/** @file "/owlcpp/include/owlcpp/rdf/query_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef QUERY_NODE_HPP_
#define QUERY_NODE_HPP_
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/iri_tags.hpp"

namespace owlcpp{

/**@return true if node is literal
*******************************************************************************/
inline bool is_literal(const Node_id nid, Triple_store const& ts) {
   Node const& node = ts[nid];
   return is_empty(node.ns_id());
}

namespace detail{
template<class Out> struct Get_value : public Visitor_node {
   Out out;

   void visit_impl(Node_iri const& node) {

   }

   void visit_impl(Node_blank const& node) {

   }

   void visit_impl(Node_bool const& node) {
      out = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_int const& node) {
      out = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_unsigned const& node) {
      out = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_double const& node) {
      out = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_string const& node) {
      out = boost::lexical_cast<Out>(node.value());
   }
};
}//namespace detail

/**
*******************************************************************************/
template<class Out> inline Out value(const Node_id nid, Triple_store const& ts) {
   Node const& node = ts[nid];
   detail::Get_value<Out> gv;
   try{
      node.accept(gv);
   }catch(...) {
      BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("conversion error")
               << Rdf_err::str1_t(to_string(node))
               << Rdf_err::nested_t(boost::current_exception())
      );
   }
   return gv.out;
}

}//namespace owlcpp
#endif /* QUERY_NODE_HPP_ */
