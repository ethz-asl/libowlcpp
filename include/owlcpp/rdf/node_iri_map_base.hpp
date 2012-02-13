/** @file "/owlcpp/include/owlcpp/rdf/node_iri_map_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_IRI_MAP_BASE_HPP_
#define NODE_IRI_MAP_BASE_HPP_
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/range.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{

/**@brief base for IRI node maps
*******************************************************************************/
class Node_iri_map_base {
public:
   typedef std::pair<Node_id, Node> value_t;
private:
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
   typedef nodes_t::iterator iterator;
   typedef iterator const_iterator;
   typedef iri_index_t::iterator iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   struct Err : public base_exception {};

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   /**@brief Find nodes in a given namespace
    @param iid namespace IRI ID
    @return iterator range for nodes in namespace iid
   */
   iri_range find(const Ns_id iid) const {
      return boost::make_iterator_range(
               nodes_.get<iri_tag>().equal_range(iid)
      );
   }

   Node_id const* find(Node const& node) const {
      node_index_t const& node_index = nodes_.get<node_tag>();
      const node_iter_t node_iter = node_index.find(node);
      if( node_iter == node_index.end() ) return 0;
      return &node_iter->first;
   }

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

   void insert(const Node_id id, Node const& node) {
      BOOST_ASSERT(
               nodes_.get<id_tag>().find(id) == nodes_.get<id_tag>().end()
      );
      BOOST_ASSERT(
               nodes_.get<node_tag>().find(node) == nodes_.get<node_tag>().end()
      );
      nodes_.insert( std::make_pair(id, node) );
   }

private:
   nodes_t nodes_;
};

}//namespace owlcpp
#endif /* NODE_IRI_MAP_BASE_HPP_ */
