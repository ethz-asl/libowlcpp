/** @file "/owlcpp/include/owlcpp/rdf/node_store.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_HPP_
#define NODE_STORE_HPP_

#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/tuple/tuple.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/iri_store.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{
class Node_tag_inserter;
}

/**@brief Store RDF nodes
*******************************************************************************/
class OWLCPP_RDF_DECL Node_store {
public:
   typedef Node_id id_type;
private:
   typedef boost::tuple<id_type, Node> entry_t;

   typedef boost::multi_index_container<
         entry_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::const_mem_fun<
                  entry_t::inherited, id_type const&, &entry_t::get<0>
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct node_tag>,
               boost::multi_index::const_mem_fun<
                  entry_t::inherited, Node const&, &entry_t::get<1>
               >
            >
         >
      > store_t;
   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef store_t::index<node_tag>::type node_index_t;
   typedef node_index_t::iterator node_iter_t;

   friend class detail::Node_tag_inserter;

protected:
   Node_store(Iri_store const& is) : tracker_(), iris_(is), store_() {init();}

public:
   struct Err : public base_exception {};
   Node_store() : tracker_(), iris_(), store_() {init();}

   Iri_store const& iri_store() const {return iris_;}
   Iri_store& iri_store() {return iris_;}
   std::size_t size() const {return store_.size();}

   id_type insert(Node const& node) {
      node_index_t const& node_index = store_.get<node_tag>();
      const node_iter_t node_iter = node_index.find(node);
      if( node_iter == node_index.end() ) {
         const id_type id = tracker_.get();
         BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
         store_.insert( boost::make_tuple(id, node) );
         return id;
      }
      return node_iter->get<0>();
   }

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(id)->get<1>();
   }

   Node const& at(const Node_id id) const {
      id_index_t const& index = store_.get<id_tag>();
      const id_iter_t iter = index.find(id);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown node ID")
               << Err::int1_t(id())
      );
      return iter->get<1>();
   }

private:
   detail::Id_tracker<id_type> tracker_;
   Iri_store iris_;
   store_t store_;

   void init();
   void insert(const Node_id id, Node const& node);
};

}//namespace owlcpp
#endif /* NODE_STORE_HPP_ */
