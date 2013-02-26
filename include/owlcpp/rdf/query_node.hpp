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
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp{

/**@return true if node is literal
*******************************************************************************/
inline bool is_literal(const Node_id nid, Triple_store const& ts) {
   Node const& node = ts[nid];
   return is_empty(node.ns_id());
}

inline Node_literal const& to_literal(Node const& node) {
   try{
      return dynamic_cast<Node_literal const&>(node);
   }catch(std::bad_cast&) {
      BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("literal node is required")
               << Rdf_err::str1_t(to_string(node))
      );
   }
}

namespace detail{
template<class Out> class Get_value : public Visitor_node {
   Triple_store const& ts_;
   Out out_;

public:
   Get_value(Triple_store const& ts): ts_(ts) {}

   Out operator()() const {return out_;}

   void visit_impl(Node_iri const& node) {
      BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("error converting IRI node to value")
               << Rdf_err::str1_t(to_string(node))
      );
   }

   void visit_impl(Node_blank const& node) {
      BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("error converting blank node to value")
               << Rdf_err::str1_t(to_string(node))
      );
   }

   void visit_impl(Node_bool const& node) {
      out_ = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_int const& node) {
      out_ = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_unsigned const& node) {
      out_ = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_double const& node) {
      out_ = boost::numeric_cast<Out>(node.value());
   }

   void visit_impl(Node_string const& node) {
      out_ = boost::lexical_cast<Out>(node.value());
   }
};

template<> class Get_value<std::string> : public Visitor_node {
   Triple_store const& ts_;
   std::string out_;

public:
   Get_value(Triple_store const& ts): ts_(ts) {}

   std::string operator()() const {return out_;}

   void visit_impl(Node_iri const& node) {
      out_ = to_string_full(node, ts_);
   }

   void visit_impl(Node_blank const& node) {
      out_ = to_string_full(node, ts_);
   }

   void visit_impl(Node_bool const& node) {
      out_ = node.value_str();
   }

   void visit_impl(Node_int const& node) {
      out_ = node.value_str();
   }

   void visit_impl(Node_unsigned const& node) {
      out_ = node.value_str();
   }

   void visit_impl(Node_double const& node) {
      out_ = node.value_str();
   }

   void visit_impl(Node_string const& node) {
      out_ = node.value();
   }
};
}//namespace detail

/**
*******************************************************************************/
template<class Out> inline Out value(const Node_id nid, Triple_store const& ts) {
   Node const& node = ts[nid];
   detail::Get_value<Out> gv(ts);
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
   return gv();
}

}//namespace owlcpp
#endif /* QUERY_NODE_HPP_ */
