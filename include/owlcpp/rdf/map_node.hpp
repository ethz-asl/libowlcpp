/** @file "/owlcpp/include/owlcpp/rdf/map_node.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_HPP_
#define MAP_NODE_HPP_
#include "owlcpp/rdf/detail/map_node_base.hpp"
#include "owlcpp/rdf/map_node_std.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Map_node {
   typedef detail::Map_node_base map_node;

public:
   struct Err : public Rdf_err {};

   typedef map_node::const_iterator const_iterator;
   typedef map_node::iterator iterator;

   template<class Nodes> explicit Map_node(Nodes const& nodes)
   : smap_(Map_node_std::get(nodes)), map_(smap_.node_id_next())
   {}

   explicit Map_node(Map_node_std const& snode)
   : smap_(snode), map_(smap_.node_id_next())
   {}

   std::size_t size() const {return map_.size();}
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}

   Node const& operator[](const Node_id id) const {
      return id < smap_.node_id_next() ? smap_[id] : map_[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      return id < smap_.node_id_next() ? smap_at(id) : map_at(id);
   }

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {
      if( id < smap_.node_id_next() ) return;
      map_.remove(id);
   }

   void clear() { map_.clear(); }

private:
   Map_node_std const& smap_;
   map_node map_;
};

}//namespace owlcpp
#endif /* MAP_NODE_HPP_ */
