/** @file "/owlcpp/include/owlcpp/rdf/node_iri_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_IRI_MAP_HPP_
#define NODE_IRI_MAP_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/range.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_iri_map {
public:
   typedef std::pair<Node_id, Node> value_t;
protected:
   static Ns_id ns_id(value_t const& v) {return v.second.ns_id();}

   typedef boost::multi_index_container<
         value_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  value_t, Node_id, &value_t::first
               >
            >,
            //blank and literal nodes may be non-unique
            boost::multi_index::hashed_non_unique<
               boost::multi_index::tag<struct node_tag>,
               boost::multi_index::member<
                  value_t, Node, &value_t::second
               >
            >,
            boost::multi_index::hashed_non_unique<
               boost::multi_index::tag<struct iri_tag>,
               boost::multi_index::global_fun<
                  value_t const&, Ns_id, &ns_id
               >
            >
         >
      > nodes_t;
   typedef nodes_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef nodes_t::index<node_tag>::type node_index_t;
   typedef node_index_t::iterator node_iter_t;
   typedef nodes_t::index<iri_tag>::type iri_index_t;

public:
   typedef Member_iterator<nodes_t::const_iterator, const Node_id, &value_t::first> const_iterator;
   typedef const_iterator iterator;
   typedef boost::iterator_range<const_iterator> range;
   typedef Member_iterator<iri_index_t::const_iterator, const Node_id, &value_t::first> iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   struct Err : public base_exception {};

   Node_iri_map() : tracker_(terms::T_empty_::id()) {
      const Node node = Node(terms::T_empty_::ns_type::id(), terms::T_empty_::name());
      nodes_.insert(std::make_pair(terms::T_empty_::id(), node));
   }

   explicit Node_iri_map(const Node_id id0)
   : tracker_(std::max(id0, terms::T_empty_::id()))
   {
      const Node node = Node(terms::T_empty_::ns_type::id(), terms::T_empty_::name());
      nodes_.insert(std::make_pair(terms::T_empty_::id(), node));
   }

   std::size_t size() const {return nodes_.size();}

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(nodes_.get<id_tag>().find(id) != nodes_.get<id_tag>().end());
      return nodes_.get<id_tag>().find(id)->second;
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Node_map::Err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      id_index_t const& index = nodes_.get<id_tag>();
      const id_iter_t iter = index.find(id);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown node ID")
               << Err::int1_t(id())
      );
      return iter->second;
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
   iri_range find(const Ns_id iid) const {
      std::pair<iri_index_t::const_iterator,iri_index_t::const_iterator> p =
               nodes_.get<iri_tag>().equal_range(iid);
      return boost::make_iterator_range(
               iri_iterator(p.first), iri_iterator(p.second)
      );
   }

   range find(Node const& node) const {
      std::pair<node_index_t::const_iterator,node_index_t::const_iterator> p =
               nodes_.get<node_tag>().equal_range(node);
      return boost::make_iterator_range(
               const_iterator(p.first), const_iterator(p.second)
      );
   }

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

   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

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
      return insert(Node(nsid, name));
   }

private:
   detail::Id_tracker<Node_id> tracker_;
   nodes_t nodes_;

protected:

   Node_id insert(Node const& node) {
      node_index_t const& n_index = nodes_.get<node_tag>();
      const node_iter_t n_iter = n_index.find(node);
      if( n_iter != n_index.end() ) return n_iter->first;
      const Node_id id = tracker_.get();
      BOOST_ASSERT(
               nodes_.get<id_tag>().find(id) == nodes_.get<id_tag>().end()
      );
      nodes_.insert( std::make_pair(id, node) );
      return id;
   }

};

}//namespace owlcpp
#endif /* NODE_IRI_MAP_HPP_ */
