/** @file "/owlcpp/include/owlcpp/rdf/node_map_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_IRI_HPP_
#define NODE_MAP_IRI_HPP_
#include "boost/assert.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/rdf/node_map_std.hpp"
#include "owlcpp/rdf/std_nodes.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_map_iri {
public:
   typedef Node_map_base::const_iterator const_iterator;
   typedef Node_map_base::iterator iterator;
   typedef Node_map_base::range range;
   typedef Node_map_base::node_iterator node_iterator;
   typedef Node_map_base::node_range node_range;
   typedef Node_map_base::ns_iterator ns_iterator;
   typedef Node_map_base::ns_range ns_range;
   typedef Rdf_err Err;

   Node_map_iri(Node_map_std const& snodes = Node_map_std::get(Nodes_none()))
   : snodes_(snodes),
     tracker_(snodes_.node_id_next())
   {}

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   Node const& operator[](const Node_id id) const {
      return id < snodes_.node_id_next() ? snodes_[id] : nodes_[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      return id < snodes_.node_id_next() ? snodes_.at(id) : nodes_.at(id);
   }

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
   ns_range find(const Ns_id iid) const {
      return snodes_.is_constant(iid) ? snodes_.find(iid) : nodes_.find(iid);
   }

   node_range find(Node const& node) const {
      return
               snodes_.is_constant(node.ns_id()) || node == Node(terms::T_empty_()) ?
                        snodes_.find(node) : nodes_.find(node);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      if( is_empty(nsid)  ) {
         if( name.empty() ) return terms::T_empty_::id();
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("empty namespace for IRI node")
                  << Err::str1_t(name)
         );
      }
      if( is_blank(nsid) )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("blank namespace for IRI node")
                  << Err::str1_t("_" + name)
         );
      return insert(nsid, name);
   }

   Node_id insert(const Ns_id nsid, std::string const& name) {
      const Node node(nsid, name);
      if( snodes_.is_constant(nsid) ) {
         const node_range r = snodes_.find(node);
         if( r ) return r.front();
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("new term cannot be inserted into standard namespace")
                  << typename Err::str1_t( name )
         );
      }
      const node_range nr = nodes_.find(node);
      if( nr ) return nr.front();
      const Node_id id = tracker_.get();
      nodes_.insert(id, node);
      return id;
   }

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {
      if( id < snodes_.node_id_next() ) return;
      nodes_.remove(id);
      tracker_.push(id);
   }

private:
   Node_map_std const& snodes_;
   detail::Id_tracker<Node_id> tracker_;
   Node_map_base nodes_;
};

}//namespace owlcpp
#endif /* NODE_MAP_IRI_HPP_ */
