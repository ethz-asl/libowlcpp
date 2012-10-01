/** @file "/owlcpp/include/owlcpp/rdf/store_concepts.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_CONCEPTS_HPP_
#define STORE_CONCEPTS_HPP_
#include <string>
#include "boost/concept/assert.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "boost/concept/detail/concept_def.hpp"

namespace owlcpp{

/**
Namespace IRI store concept
*******************************************************************************/
BOOST_concept(Ns_store, (T)) {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_ns_type map_ns_type;
   typedef typename traits::ns_id_type ns_id_type;

   BOOST_CONCEPT_USAGE(Ns_store) {
      const bool b = t_.valid(nsid_);
      boost::ignore_unused_variable_warning(b);

      str_ = t_[nsid_];
      str_ = t_.at(nsid_);
      str_ = t_.prefix(nsid_);

      map_ns_type const& map_ns = t_.map_ns();
      boost::ignore_unused_variable_warning(map_ns);

      nsid_ = t_.insert_ns(str_);

      ns_id_type const* nsid = t_.find_ns(str_);
      nsid = t_.find_prefix(str_);
      boost::ignore_unused_variable_warning(nsid);
   }

private:
   T t_;
   std::string str_;
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

   BOOST_CONCEPT_USAGE(Iri_node_store) {
      const bool b = t_.valid(nid_);
      boost::ignore_unused_variable_warning(b);

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
Namespace IRI and node IRI store concept
*******************************************************************************/
BOOST_concept(Ns_iri_node_store, (T)) {
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
