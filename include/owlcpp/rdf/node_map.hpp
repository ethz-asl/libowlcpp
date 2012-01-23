/** @file "/owlcpp/include/owlcpp/rdf/node_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_HPP_
#define NODE_MAP_HPP_
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{

class Node_tag_inserter;

}

/**@brief Store RDF nodes
*******************************************************************************/
class OWLCPP_RDF_DECL Node_map {
public:
   typedef Node_id id_type;
private:
   typedef std::pair<id_type, Node> entry_t;

   typedef boost::multi_index_container<
         entry_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  entry_t, id_type, &entry_t::first
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct node_tag>,
               boost::multi_index::member<
                  entry_t, Node, &entry_t::second
               >
            >
         >
      > store_t;
   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef store_t::index<node_tag>::type node_index_t;
   typedef node_index_t::iterator node_iter_t;

   friend class detail::Node_tag_inserter;

public:
   typedef store_t::iterator iterator;

   struct Err : public base_exception {};
   Node_map();
   std::size_t size() const {return store_.size();}

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(id)->second;
   }

   //TODO: implement range_t find(const Ns_id) const;

   Node_id const* find(Node const& node) const {
      node_index_t const& node_index = store_.get<node_tag>();
      const node_iter_t node_iter = node_index.find(node);
      if( node_iter == node_index.end() ) return 0;
      return &node_iter->first;
   }

   Node const& at(const Node_id id) const {
      id_index_t const& index = store_.get<id_tag>();
      const id_iter_t iter = index.find(id);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown node ID")
               << Err::int1_t(id())
      );
      return iter->second;
   }

   void remove(const id_type id);
   void remove(Node const& node);
   iterator begin() const {return store_.begin();}
   iterator end() const {return store_.end();}

   id_type insert(Node const& node) {
      node_index_t const& node_index = store_.get<node_tag>();
      const node_iter_t node_iter = node_index.find(node);
      if( node_iter == node_index.end() ) {
         const id_type id = tracker_.get();
         BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
         store_.insert( std::make_pair(id, node) );
         return id;
      }
      return node_iter->first;
   }

private:
   detail::Id_tracker<id_type> tracker_;
   store_t store_;

   void insert(const Node_id id, Node const& node);
};

}//namespace owlcpp
#endif /* NODE_MAP_HPP_ */
