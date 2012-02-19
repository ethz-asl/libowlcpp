/** @file "/owlcpp/include/owlcpp/rdf/node_map_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_BASE_HPP_
#define NODE_MAP_BASE_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/range.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{
/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_map_base {
public:
   typedef Node_id id_type;
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
   typedef Member_iterator<node_index_t::const_iterator, const Node_id, &value_t::first> node_iterator;
   typedef boost::iterator_range<node_iterator> node_range;
   typedef Member_iterator<iri_index_t::const_iterator, const Node_id, &value_t::first> ns_iterator;
   typedef boost::iterator_range<ns_iterator> ns_range;

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(nodes_.get<id_tag>().find(id) != nodes_.get<id_tag>().end());
      return nodes_.get<id_tag>().find(id)->second;
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      id_index_t const& index = nodes_.get<id_tag>();
      const id_iter_t iter = index.find(id);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("unknown node ID")
               << Rdf_err::int1_t(id())
      );
      return iter->second;
   }

   /**@brief Find nodes in a given namespace
    @param iid namespace IRI ID
    @return iterator range for nodes in namespace iid
   */
   ns_range find(const Ns_id iid) const {
      std::pair<iri_index_t::const_iterator,iri_index_t::const_iterator> p =
               nodes_.get<iri_tag>().equal_range(iid);
      return boost::make_iterator_range(
               ns_iterator(p.first), ns_iterator(p.second)
      );
   }

   node_range find(Node const& node) const {
      std::pair<node_index_t::const_iterator,node_index_t::const_iterator> p =
               nodes_.get<node_tag>().equal_range(node);
      return boost::make_iterator_range(
               node_iterator(p.first), node_iterator(p.second)
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
   }

   void insert(const Node_id nid, Node const& node) {
      nodes_.insert(std::make_pair(nid, node));
   }

   template<class Tag> void insert_tag(Tag const&) {
      insert( Tag::id(), Node(Tag()) );
   }

private:
   nodes_t nodes_;
};

}//namespace owlcpp
#endif /* NODE_MAP_BASE_HPP_ */
