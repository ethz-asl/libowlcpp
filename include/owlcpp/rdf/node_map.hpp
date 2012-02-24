/** @file "/owlcpp/include/owlcpp/rdf/node_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_HPP_
#define NODE_MAP_HPP_
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/rdf/nodes_iri_std_crtpb.hpp"
#include "owlcpp/rdf/nodes_blank_crtpb.hpp"
#include "owlcpp/rdf/nodes_literal_crtpb.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_map :
   public Nodes_iri_std_crtpb<Node_map>,
   public Nodes_blank_crtpb<Node_map>,
   public Nodes_literal_crtpb<Node_map>
   {
   friend class Nodes_iri_std_crtpb<Node_map>;
   friend class Nodes_blank_crtpb<Node_map>;
   friend class Nodes_literal_crtpb<Node_map>;

   typedef detail::Id_tracker<Node_id> tracker_t;

   Node_map_base& node_map() {return nodes_;}
   Node_map_base const& node_map() const {return nodes_;}

   Node_id insert_new(const Ns_id nsid, std::string const& val) {
      const Node_id nid = tracker_.get();
      nodes_.insert(nid, Node(nsid, val));
      return nid;
   }

public:
   struct Err : public Rdf_err {};

   typedef Node_map_base::const_iterator const_iterator;
   typedef Node_map_base::iterator iterator;
   typedef Node_map_base::range range;

   Node_map(Node_map_std const& snodes = Node_map_std::get(Nodes_none()))
   : Nodes_iri_std_crtpb<Node_map>(snodes),
     tracker_(snode_id_next())
     {}

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {
      if( id < snode_id_next() ) return;
      Node const& node = nodes_[id];
      if( is_blank(node.ns_id()) ) remove_blank(id);
      else if( is_empty(node.ns_id()) ) remove_literal(id);
      nodes_.remove(id);
      tracker_.push(id);
   }

private:
   tracker_t tracker_;
   Node_map_base nodes_;

};

}//namespace owlcpp
#endif /* NODE_MAP_HPP_ */
