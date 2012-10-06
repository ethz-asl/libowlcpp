/** @file "/owlcpp/include/owlcpp/rdf/map_node_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_IRI_HPP_
#define MAP_NODE_IRI_HPP_
#include <string>
#include <vector>
#include "boost/assert.hpp"
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/hash_node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{

/**@brief Store RDF IRI nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Map_node_iri {
public:
   typedef Node_id id_type;
   typedef Node_iri node_type;
private:
   typedef boost::unordered_map<Node_iri, Node_id> map_t;
   typedef map_t::iterator map_iter_t;
   typedef map_t::const_iterator map_citer_t;

   typedef std::vector<Node_iri const*> vector_t;

   typedef std::pair<map_iter_t,bool> insert_t;

public:
   typedef Member_iterator<
            map_citer_t,
            Node_id const,
            &std::pair<const Node_iri,Node_id>::second
   > iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   std::size_t size() const { return m_.size(); }
   const_iterator begin() const {return m_.begin();}
   const_iterator end() const {return m_.end();}
   bool empty() const {return m_.empty();}

   bool valid(const Node_id id) const {
      if( id < detail::max_std_node_id() ) return false;
      const std::size_t n = sz(id);
      return n < v_.size() && v_[n];
   }

   Node_iri const& operator[](const Node_id id) const {
      BOOST_ASSERT(valid(id));
      return get(id);
   }

   Node_iri const& at(const Node_id id) const {
      if( ! valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid node ID")
               << Err::int1_t(id())
      );
      return get(id);
   }

   Node_iri const* find(const Node_id id) const {
      return
               id < detail::max_std_node_id() ||
               id() >= v_.size() + detail::max_std_node_id()() ?
                        0 :
                        v_[sz(id)]
                           ;
   }

   Node_id const* find(const Ns_id ns, std::string const& val) const {
      return find(Node_iri(ns,val));
   }

   Node_id const* find(Node_iri const& node) const {
      const map_citer_t i = m_.find(node);
      return i == m_.end() ? 0 : &i->second;
   }

   Node_id insert(Node_iri const& node) {
      insert_t ip = m_.emplace(node, Node_id());
      if( ip.second ) {
         const Node_id id = make_id(ip.first);
         ip.first->second = id;
         return id;
      }
      return ip.first->second;
   }

   void insert(const Node_id id, Node_iri const& node) {
      if( id < detail::max_std_node_id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid ID")
               << Err::int1_t(id())
      );

      if( Node_id const* id0 = find(node) ) {
         if( *id0 == id ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("node already exists")
                  << Err::int1_t(node.ns_id()())
                  << Err::str1_t(node.name())
         );
      }

      if( valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("node ID not available")
               << Err::int1_t(id())
      );

      insert_t ip = m_.emplace(node, id);
      BOOST_ASSERT(ip.second);
      //ignore erased_
      const std::size_t n = sz(id);
      if( n < v_.size() ) {
         v_[n] = &ip.first->first;
      } else {
         v_.resize(n,0);
         v_.push_back(&ip.first->first);
      }
   }

   Node_id insert(const Ns_id ns, std::string const& val) {
      return insert(Node_iri(ns, val));
   }

   void insert(const Node_id id, const Ns_id ns, std::string const& val) {
      insert(id, Node_iri(ns, val));
   }

   void remove(const Node_id id) {
      BOOST_ASSERT(valid(id));
      const std::size_t n = m_.erase(get(id));
      BOOST_ASSERT(n);
      erased_.push_back(id);
      v_[sz(id)] = 0;
   }

   void clear() {
      erased_.clear();
      m_.clear();
      v_.clear();
   }

private:
   vector_t v_;
   map_t m_;
   std::vector<Node_id> erased_;

   std::size_t sz(const Node_id id) const {
      BOOST_ASSERT(id >= detail::max_std_node_id());
      return id() - detail::max_std_node_id()();
   }

   Node_id nid(const std::size_t n) const {
      return Node_id(n + detail::max_std_node_id()());
   }

   Node_iri const& get(const Node_id id) const {return *v_[sz(id)];}

   Node_id make_id(const map_citer_t i) {
      Node_iri const*const p = &i->first;
      if( erased_.empty() ) {
         const Node_id id = nid(v_.size());
         v_.push_back(p);
         return id;
      }
      const Node_id id = erased_.back();
      erased_.pop_back();
      const std::size_t n = sz(id);
      BOOST_ASSERT(v_.size() > n && ! v_[n]);
      v_[n] = p;
      return id;
   }

};

}//namespace owlcpp
#endif /* MAP_NODE_IRI_HPP_ */
