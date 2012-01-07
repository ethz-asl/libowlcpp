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
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
*******************************************************************************/
class Node_store {
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

protected:
   Node_store(Iri_store const& is, const std::size_t n0)
   : tracker_(n0), iris_(is), store_() {}

   void insert(const Node_id nid, Node const& node) {
      BOOST_ASSERT(
               store_.get<id_tag>().find(nid) == store_.get<id_tag>().end()
      );
      BOOST_ASSERT(
               store_.get<node_tag>().find(node) == store_.get<node_tag>().end()
      );
      store_.insert( boost::make_tuple(nid, node) );
   }

public:
   struct Err : public base_exception {};
   Node_store() : tracker_(0), iris_(), store_() {}

   Iri_store const& iri_store() const {return iris_;}
   Iri_store& iri_store() {return iris_;}
   std::size_t size() const {return store_.size();}

   id_type insert(Node const& n) {

      node_index_t const& node_index = store_.get<node_tag>();
      const node_iter_t node_iter = node_index.find(n);
      if( node_iter == node_index.end() ) {
         const id_type id = tracker_.get();
         BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
         store_.insert( boost::make_tuple(id, n) );
         return id;
      }
      return node_iter->get<0>();
   }

   Node const& operator[](const Node_id nid) const {
      BOOST_ASSERT(store_.get<id_tag>().find(nid) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(nid)->get<1>();
   }

   Node const& at(const Node_id nid) const {
      id_index_t const& index = store_.get<id_tag>();
      const id_iter_t iter = index.find(nid);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown node ID")
               << Err::int1_t(nid())
      );
      return iter->get<1>();
   }

private:
   detail::Id_tracker<id_type> tracker_;
   Iri_store iris_;
   store_t store_;
};

}//namespace owlcpp
#endif /* NODE_STORE_HPP_ */
