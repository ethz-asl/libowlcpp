/** @file "/owlcpp/include/owlcpp/rdf/node_iri_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_IRI_MAP_HPP_
#define NODE_IRI_MAP_HPP_
#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/rdf/detail/node_iri_cmap.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_iri_map {
   typedef detail::Node_iri_cmap node_map_t;
public:
   typedef node_map_t::const_iterator const_iterator;
   typedef node_map_t::iterator iterator;
   typedef node_map_t::range range;
   typedef node_map_t::node_iterator node_iterator;
   typedef node_map_t::node_range node_range;
   typedef node_map_t::ns_iterator ns_iterator;
   typedef node_map_t::ns_range ns_range;

   struct Err : public base_exception {};

   Node_iri_map() : tracker_(terms::T_empty_::id()) {
      map_.insert_tag(terms::T_empty_());
   }

   explicit Node_iri_map(const Node_id id0)
   : tracker_(std::max(id0, terms::T_empty_::id()))
   {
      map_.insert_tag(terms::T_empty_());
   }

   std::size_t size() const {return map_.size();}
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}
   Node const& operator[](const Node_id id) const {return map_[id];}

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Node_map::Err if @b id does not exist
   */
   Node const& at(const Node_id id) const {return map_.at(id);}

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
   ns_range find(const Ns_id iid) const {return map_.find(iid);}

   node_range find(Node const& node) const {return map_.find(node);}


   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {
      map_.remove(id);
      tracker_.push(id);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      if( nsid == terms::N_empty::id() )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("empty namespace for IRI node")
                  << Err::str1_t(name)
         );
      if( nsid == terms::N_blank::id() )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("blank namespace for IRI node")
                  << Err::str1_t("_" + name)
         );
      const Node node(nsid, name);
      const node_range nr = map_.find(node);
      if( nr ) return nr.front();
      return insert(node);
   }

private:
   detail::Id_tracker<Node_id> tracker_;
   node_map_t map_;

protected:
   Node_id insert(Node const& node) {
      const Node_id id = tracker_.get();
      map_.insert(id, node);
      return id;
   }

};

}//namespace owlcpp
#endif /* NODE_IRI_MAP_HPP_ */
