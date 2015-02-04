/** @file "/owlcpp/include/owlcpp/rdf/store_concepts.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_CONCEPTS_HPP_
#define STORE_CONCEPTS_HPP_
#include <string>
#include "boost/concept_check.hpp"
#include "boost/concept/assert.hpp"
#include "boost/concept/detail/concept_def.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/ns_iri.hpp"

namespace owlcpp{
class Ns_iri;

/**
Namespace IRI store concept
*******************************************************************************/
BOOST_concept(Ns_store, (T)) {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_ns_type map_ns_type;
   typedef typename traits::ns_id_type ns_id_type;

   BOOST_CONCEPT_USAGE(Ns_store) {
      ns_iri_ = t_[nsid_];
      ns_iri_ = t_.at(nsid_);
      ns_iri_ = *t_.find(nsid_);
      pref_ = t_.prefix(nsid_);

      map_ns_type const& map_ns = t_.map_ns();
      boost::ignore_unused_variable_warning(map_ns);

      nsid_ = t_.insert(ns_iri_);

      ns_id_type const* nsid = t_.find(ns_iri_);
      nsid = t_.find_prefix(pref_);
      boost::ignore_unused_variable_warning(nsid);
   }

private:
   T t_;
   Ns_iri ns_iri_;
   std::string pref_;
   ns_id_type nsid_;
};

/**
IRI node store concept
*******************************************************************************/
BOOST_concept(Iri_node_store, (T)) {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_node_type map_node_type;
   typedef typename traits::node_id_type node_id_type;
   typedef typename traits::ns_id_type ns_id_type;
   typedef typename traits::node_type node_type;

   BOOST_CONCEPT_USAGE(Iri_node_store) {
      node_type const* node = t_.find(nid_);
      boost::ignore_unused_variable_warning(node);

      map_node_type const& map_node = t_.map_node();
      boost::ignore_unused_variable_warning(map_node);

      nid_ = t_.insert_node_iri(nsid_, str_);

      node_id_type const* nid = t_.find_node_iri(nsid_, str_);
      boost::ignore_unused_variable_warning(nid);
   }

private:
   T t_;
   std::string str_;
   node_id_type nid_;
   ns_id_type nsid_;
};

/**
Document info store concept
*******************************************************************************/
BOOST_concept(Doc_store, (T)) {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_doc_type map_doc_type;
   typedef typename traits::doc_id_type doc_id_type;
   typedef typename traits::node_id_type node_id_type;
   typedef typename traits::doc_type doc_type;

   BOOST_CONCEPT_USAGE(Doc_store) {
      doc_type const* doc = t_.find(did_);
      boost::ignore_unused_variable_warning(doc);

      map_doc_type const& map_doc = t_.map_doc();
      boost::ignore_unused_variable_warning(map_doc);

      std::pair<doc_id_type,bool> p = t_.insert_doc(nid_, str_, nid_);
      p = t_.insert_doc(nid_, str_);
      p = t_.insert_doc(nid_);
      p = t_.insert_doc(str_, str_, str_);
      p = t_.insert_doc(str_, str_);
      p = t_.insert_doc(str_);
   }

private:
   T t_;
   std::string str_;
   node_id_type nid_;
   doc_id_type did_;
};

/**
IRI, blank and literal node store concept
*******************************************************************************/
BOOST_concept(Node_store, (T)) : Iri_node_store<T> {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_node_type map_node_type;
   typedef typename traits::node_id_type node_id_type;
   typedef typename traits::ns_id_type ns_id_type;
   typedef typename traits::doc_id_type doc_id_type;

   BOOST_CONCEPT_USAGE(Node_store) {
      const std::string str;
      node_id_type nid = t_.insert_literal(str, str, str);
      nid = t_.insert_literal(str, str);

      const Node_id nid_dt;
      nid = t_.insert_literal(str, nid_dt, str);
      nid = t_.insert_literal(str, nid_dt);

      node_id_type const* nidp = t_.find_literal(str, str, str);
      nidp = t_.find_literal(str, str);
      boost::ignore_unused_variable_warning(nidp);

      const unsigned n = 0;
      doc_id_type did;
      nid = t_.insert_blank(n, did);
      nidp = t_.find_blank(n, did);
   }

private:
   T t_;
};

/**
Namespace IRI and node IRI store concept
*******************************************************************************/
BOOST_concept(Ns_iri_node_store, (T)) : Iri_node_store<T>, Ns_store<T> {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_node_type map_node_type;
   typedef typename traits::node_id_type node_id_type;

   BOOST_CONCEPT_USAGE(Ns_iri_node_store) {

      nid_ = t_.insert_node_iri(str_);

      node_id_type const* nid = t_.find_node_iri(str_);
      boost::ignore_unused_variable_warning(nid);
   }

private:
   T t_;
   std::string str_;
   node_id_type nid_;
};

}//namespace owlcpp
#endif /* STORE_CONCEPTS_HPP_ */
