/** @file "/owlcpp/include/owlcpp/rdf/node_iri_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_IRI_MAP_HPP_
#define NODE_IRI_MAP_HPP_
#include "owlcpp/rdf/node_iri_map_base.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_iri_map {
public:
   typedef Node_iri_map_base::value_t value_t;

public:
   typedef Node_iri_map_base::iterator iterator;
   typedef iterator const_iterator;
   typedef Node_iri_map_base::iri_iterator iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   struct Err : public base_exception {};

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   /**@brief Find nodes in a given namespace
    @param iid namespace IRI ID
    @return iterator range for nodes in namespace iid
    @details For example:
    @code
    find(terms::N_owl::id()) //returns range of all nodes in OWL namespace
    find(terms::N_empty::id()) //returns range of all literal nodes
    find(terms::N_blank::id()) //returns range of all blank nodes
    @endcode
   */
   iri_range find(const Ns_id iid) const {return nodes_.find(iid);}
   Node_id const* find(Node const& node) const {return nodes_.find(node);}
   Node const& operator[](const Node_id id) const {return nodes_[id];}

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Node_map::Err if @b id does not exist
   */
   Node const& at(const Node_id id) const {return nodes_.at(id);}

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {
      id_index_t & id_index = nodes_.get<id_tag>();
      id_iter_t i = id_index.find(id);
      BOOST_ASSERT( i != id_index.end() );
      id_index.erase(i);
      tracker_.push(id);
   }

   void remove(Node const& node) {
      node_index_t& node_index = nodes_.get<node_tag>();
      node_iter_t i = node_index.find(node);
      if( i == node_index.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("removing non-existing node")
               << Err::str1_t(node.value_str())
      );
      const Node_id id = i->first;
      node_index.erase(i);
      tracker_.push(id);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      node_index_t const& n_index = nodes_.get<node_tag>();
      const Node node(nsid, name);
      const node_iter_t n_iter = n_index.find(node);
      if( n_iter != n_index.end() ) return n_iter->first;
      const Node_id id = tracker_.get();
      insert(id, node);
      return id;
   }

private:
   detail::Id_tracker<Node_id> tracker_;
   Node_iri_map_base nodes_;

   void insert(const Node_id id, Node const& node) {
      BOOST_ASSERT(
               nodes_.get<id_tag>().find(id) == nodes_.get<id_tag>().end()
      );
      BOOST_ASSERT(
               nodes_.get<node_tag>().find(node) == nodes_.get<node_tag>().end()
      );
      nodes_.insert( std::make_pair(id, node) );
      tracker_.ensure_min(id);
   }

};

}//namespace owlcpp
#endif /* NODE_IRI_MAP_HPP_ */
