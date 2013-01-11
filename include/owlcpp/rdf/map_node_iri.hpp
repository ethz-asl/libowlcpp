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
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include "boost/concept_check.hpp"

#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/hash_node.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/detail/iterator_member_pair.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

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
   typedef std::pair<Node_iri, Node_id> map_element_t;
   typedef map_t::iterator map_iter_t;
   typedef map_t::const_iterator map_citer_t;

   typedef std::vector<Node_iri const*> vector_t;

   typedef std::pair<map_iter_t,bool> insert_t;

public:
   typedef Iterator_member_pair<map_citer_t, Node_id const, 2> iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   Map_node_iri(const Node_id min_id = detail::min_node_id())
   : vid_(), map_(), erased_(), min_id_(min_id)
   {}

   Map_node_iri(Map_node_iri const& mni)
   : vid_(mni.vid_.size()), map_(), erased_(mni.erased_), min_id_(mni.min_id_)
   {
      copy(mni);
   }

   Map_node_iri& operator=(Map_node_iri const& mni) {
      clear();
      min_id_ = mni.min_id_;
      vid_.resize(mni.vid_.size());
      copy(mni);
      erased_ = mni.erased_;
      return *this;
   }

   std::size_t size() const { return map_.size(); }
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}
   bool empty() const {return map_.empty();}

   bool valid(const Node_id id) const {
      if( id < min_id_ ) return false;
      const std::size_t n = sz(id);
      return n < vid_.size() && vid_[n];
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
               id < min_id_ ||
               id() >= vid_.size() + min_id_() ?
                        0 :
                        vid_[sz(id)]
                           ;
   }

   Node_id const* find_iri(const Ns_id ns, std::string const& val) const {
      return find(Node_iri(ns,val));
   }

   Node_id const* find(Node_iri const& node) const {
      const map_citer_t i = map_.find(node);
      return i == map_.end() ? 0 : &i->second;
   }

   Node_id insert(Node_iri const& node) {
      insert_t ip = map_.emplace(node, Node_id());
      if( ip.second ) {
         const Node_id id = make_id(ip.first);
         ip.first->second = id;
         return id;
      }
      return ip.first->second;
   }

   void insert(const Node_id id, Node_iri const& node) {
      if( id < min_id_ ) BOOST_THROW_EXCEPTION(
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
                  << Err::str1_t(node.fragment())
         );
      }

      if( valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("node ID not available")
               << Err::int1_t(id())
      );

      insert_t ip = map_.emplace(node, id);
      BOOST_ASSERT(ip.second);
      //ignore erased_
      const std::size_t n = sz(id);
      if( n < vid_.size() ) {
         vid_[n] = &ip.first->first;
      } else {
         vid_.resize(n,0);
         vid_.push_back(&ip.first->first);
      }
   }

   Node_id insert_iri(const Ns_id ns, std::string const& val) {
      return insert(Node_iri(ns, val));
   }

   void insert_iri(const Node_id id, const Ns_id ns, std::string const& val) {
      insert(id, Node_iri(ns, val));
   }

   void remove(const Node_id id) {
      BOOST_ASSERT(valid(id));
      const std::size_t n = map_.erase(get(id));
      boost::ignore_unused_variable_warning(n);
      BOOST_ASSERT(n);
      erased_.push_back(id);
      vid_[sz(id)] = 0;
   }

   void clear() {
      erased_.clear();
      map_.clear();
      vid_.clear();
   }

private:
   vector_t vid_;
   map_t map_;
   std::vector<Node_id> erased_;
   Node_id min_id_;

   std::size_t sz(const Node_id id) const {
      BOOST_ASSERT(id >= min_id_);
      return id() - min_id_();
   }

   Node_id nid(const std::size_t n) const {
      return Node_id(n + min_id_());
   }

   Node_iri const& get(const Node_id id) const {return *vid_[sz(id)];}

   Node_id make_id(const map_citer_t i) {
      Node_iri const*const p = &i->first;
      if( erased_.empty() ) {
         const Node_id id = nid(vid_.size());
         vid_.push_back(p);
         return id;
      }
      const Node_id id = erased_.back();
      erased_.pop_back();
      const std::size_t n = sz(id);
      BOOST_ASSERT(vid_.size() > n && ! vid_[n]);
      vid_[n] = p;
      return id;
   }

   void copy(Map_node_iri const& mni) {
      BOOST_ASSERT(min_id_ == mni.min_id_ && "base ID should be same");
      BOOST_FOREACH(map_element_t const& p, mni.map_) {
         insert_t ip = map_.insert(p);
         BOOST_ASSERT(ip.second && "duplicate node");
         vid_[sz(p.second)] = &(ip.first->first);
      }
   }

};

}//namespace owlcpp
#endif /* MAP_NODE_IRI_HPP_ */
