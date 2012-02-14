/** @file "/owlcpp/include/owlcpp/rdf/owl_terms.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWL_TERMS_HPP_
#define OWL_TERMS_HPP_
#include <vector>
#include <string>
#include <functional>
#include "boost/assert.hpp"
#include "boost/range.hpp"
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**@brief
*******************************************************************************/
class OWLCPP_RDF_DECL Owl_terms {

   struct Node_name_hash : public std::unary_function<Node,std::size_t> {
      std::size_t operator()(Node const& n) const {
         return boost::hash_value(n.value_str());
      }
   };

   struct Node_name_equal : public std::binary_function<Node,Node,bool> {
      bool operator()(Node const& n1, Node const& n2) const {
         return n1.value_str() == n2.value_str();
      }
   };

   typedef boost::unordered_map<
            Node, Node_id, Node_name_hash, Node_name_equal
            >
   node_map_t;
   typedef node_map_t::const_iterator node_iter_t;
   typedef std::vector<Node const*> node_index_t;

   class Node_tag_inserter {
      Node_tag_inserter();
   public:
      explicit Node_tag_inserter(node_map_t& map, node_index_t& index)
      : map_(&map), index_(&index) {}

      template<class T> void operator()(T const&) const {
         const Node node(T::ns_type::id(), T::name());
         const node_iter_t i = map_->insert(std::make_pair(node, T::id())).first;
         (*index_)[T::id()()] = &i->first;
      }

   private:
      mutable node_map_t* map_;
      mutable node_index_t* index_;
   };

   Owl_terms();
   Owl_terms(Owl_terms const&); //not implemented

public:
   struct Err : public base_exception {};

   static Owl_terms const& get() {
      static const Owl_terms ot;
      return ot;
   }

   static bool is_owl(const Ns_id ns) {
      switch ( ns() ) {
      case terms::N_owl::index:
      case terms::N_rdf::index:
      case terms::N_rdfs::index:
      case terms::N_xsd::index:
         return true;
      default:
         return false;
      }
   }

   static bool is_owl(const Node_id id) {return id <= max_;}
   static Node_id max_id() {return max_;}

   std::size_t size() const {return node_index_.size();}

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(id() < node_index_.size());
      BOOST_ASSERT(node_index_[id()]);
      return *node_index_[id()];
   }

   Node const& at(const Node_id id) const {
      if( id() >= node_index_.size() || ! node_index_[id()] )
         BOOST_THROW_EXCEPTION(
                        Err()
                        << Err::msg_t("unknown node ID")
                        << Err::int1_t(id())
         );
      return *node_index_[id()];
   }

   Node_id const* find(Node const& node) const {
      switch ( node.ns_id()() ) {
      case terms::N_owl::index: {
         const node_iter_t i = nodes_owl_.find(node);
         return i == nodes_owl_.end() ? 0 : &i->second;
      }
      case terms::N_rdf::index: {
         const node_iter_t i = nodes_rdf_.find(node);
         return i == nodes_rdf_.end() ? 0 : &i->second;
      }
      case terms::N_rdfs::index: {
         const node_iter_t i = nodes_rdfs_.find(node);
         return i == nodes_rdfs_.end() ? 0 : &i->second;
      }
      case terms::N_xsd::index: {
         const node_iter_t i = nodes_xsd_.find(node);
         return i == nodes_xsd_.end() ? 0 : &i->second;
      }
      case terms::N_empty::index:
         return node.value_str() == terms::T_empty_::name() ? &empty_id_ : 0;
      default:
         return 0;
      }
   }

private:
   static const Node_id max_;
   node_index_t node_index_;
   const Node empty_node_;
   const Node_id empty_id_;
   node_map_t nodes_rdfs_;
   node_map_t nodes_rdf_;
   node_map_t nodes_xsd_;
   node_map_t nodes_owl_;
};

}//namespace owlcpp
#endif /* OWL_TERMS_HPP_ */
