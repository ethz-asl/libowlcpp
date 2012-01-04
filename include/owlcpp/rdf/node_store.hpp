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
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/for_each.hpp"
#include "boost/scoped_ptr.hpp"

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/iri_store.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{

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

   class Store_node_tags {
   public:
      Store_node_tags(Node_store& store) : nstor_(store) {}
      template<class T> void operator()(const T&) const {
         BOOST_ASSERT(
                  nstor_.store_.get<id_tag>().find(T::id()) ==
                           nstor_.store_.get<id_tag>().end()
                  );
         BOOST_ASSERT(
                  nstor_.store_.get<node_tag>().find(Node(T::ns_type::id(), T::name())) ==
                           nstor_.store_.get<node_tag>().end()
                  );
         nstor_.store_.insert(
                  boost::make_tuple( T::id(), Node(T::ns_type::id(), T::name()) )
         );
      }
   private:
      mutable Node_store& nstor_;
   };

   Node_store(const std::size_t n0) : tracker_(n0), iris_(), store_() {}

public:
   static Node_store owl();
   Node_store() : tracker_(0), iris_(new Iri_store()), store_() {}
   Node_store(Node_store const& ns)
   :
      tracker_(ns.tracker_),
      iris_(new Iri_store(*ns.iris_)),
      store_(ns.store_)
   {}

   Iri_store const& iri_store() const {return *iris_;}
   Iri_store& iri_store() {return *iris_;}
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
      return store_.get<id_tag>().find(nid)->get<1>();
   }

private:
   detail::Id_tracker<id_type> tracker_;
   boost::scoped_ptr<Iri_store> iris_;
   store_t store_;
};

}//namespace owlcpp
#endif /* NODE_STORE_HPP_ */
