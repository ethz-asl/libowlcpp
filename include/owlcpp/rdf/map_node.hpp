/** @file "/owlcpp/include/owlcpp/rdf/detail/map_node_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_BASE_HPP_
#define MAP_NODE_BASE_HPP_
#include <string>
#include <vector>
#include "boost/assert.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/ptr_container/indirect_fun.hpp"
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{ namespace detail{

/**@brief Store polymorphic RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Map_node {
public:
   typedef Node_id id_type;
   typedef Node node_type;
private:
   typedef boost::ptr_vector<boost::nullable<Node> > vector_t;
   typedef boost::unordered_map<
            Node const*,
            Node_id,
            boost::indirect_fun<boost::hash<Node> >,
            boost::indirect_fun<std::equal_to<Node> >
   > map_t;

   typedef map_t::iterator map_iter_t;
   typedef map_t::const_iterator map_citer_t;
   typedef std::pair<map_iter_t,bool> insert_t;

public:
   typedef Member_iterator<
            map_citer_t,
            Node_id const,
            &std::pair<Node const*const,Node_id>::second
   > iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   Map_node(const Node_id id0) : n0_(id0()) {}

   std::size_t size() const { return m_.size(); }
   const_iterator begin() const {return m_.begin();}
   const_iterator end() const {return m_.end();}
   bool empty() const {return m_.empty();}

   bool have(const Node_id id) const {return valid(id);}

   Node const& operator[](const Node_id id) const {return get(id);}

   Node const& at(const Node_id id) const {
      if( ! valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid node ID")
               << Err::int1_t(id())
      );
      return get(id);
   }

   Node const* find(const Node_id id) const {
      if( id() < n0_ || id() >= v_.size() + n0_ ) return 0;
      return  &v_[sz(id)];
   }

   Node_id const* find(Node const& node) const {
      const map_citer_t i = m_.find(&node);
      return i == m_.end() ? 0 : &i->second;
   }

   Node_id const* find_iri(const Ns_id ns, std::string const& val) const {
      return find(Node_iri(ns, val));
   }

   Node_id const* find_literal(
            std::string const& val, const Node_id dt, std::string const& lang
   ) const {
      return find(Node_literal(val, dt, lang));
   }

   Node_id const* find_blank(const unsigned n, const Doc_id doc) const {
      return find(Node_blank(n, doc));
   }

   Node_id insert_iri(const Ns_id ns, std::string const& val) {
      //todo: do not new if node already stored
      std::auto_ptr<Node> np(new Node_iri(ns,val));
      return insert(np);
   }

   Node_id insert_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id(),
            std::string const& lang = ""
   ) {
      //todo: do not new if node already stored
      std::auto_ptr<Node> np(new Node_literal(val, dt, lang));
      return insert(np);
   }

   Node_id insert_blank(const unsigned n, const Doc_id doc) {
      //todo: do not new if node already stored
      std::auto_ptr<Node> np(new Node_blank(n, doc));
      return insert(np);
   }

   std::auto_ptr<Node> remove(const Node_id id) {
      BOOST_ASSERT(valid(id));
      const std::size_t n = m_.erase(&get(id));
      BOOST_ASSERT(n);
      erased_.push_back(id);
      return std::auto_ptr<Node>( v_.replace(sz(id), 0).release() );
   }

   void clear() {
      erased_.clear();
      m_.clear();
      v_.clear();
   }

private:
   std::size_t n0_;
   vector_t v_;
   map_t m_;
   std::vector<Node_id> erased_;

   std::size_t sz(const Node_id id) const {
      BOOST_ASSERT(id() >= n0_);
      return id() - n0_;
   }

   Node_id nid(const std::size_t n) const {return Node_id(n + n0_);}

   Node const& get(const Node_id id) const {return v_[sz(id)];}

   bool valid(const Node_id id) const {
      if( id() < n0_ ) return false;
      const std::size_t n = sz(id);
      return n < v_.size() && ! v_.is_null(n);
   }

   Node_id make_id(std::auto_ptr<Node> np) {
      if( erased_.empty() ) {
         const Node_id id = nid(v_.size());
         v_.push_back(np);
         return id;
      }
      const Node_id id = erased_.back();
      erased_.pop_back();
      const std::size_t n = sz(id);
      BOOST_ASSERT(v_.is_null(n));
      v_.replace(n, np);
      return id;
   }

   Node_id insert(std::auto_ptr<Node> np) {
      insert_t ip = m_.emplace(np.get(), Node_id());
      if( ip.second ) {
         const Node_id id = make_id(np);
         ip.first->second = id;
         return id;
      }
      return ip.first->second;
   }

   void insert(const Node_id id, std::auto_ptr<Node> np) {
      Node_id const* id0 = find(*np);
      if( id0 ) {
         if( *id0 == id ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("node already exists")
                  << Err::int1_t(np->ns_id()())
                  << Err::str1_t(np->value_str())
         );
      }
      if( valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("node ID is reserved")
               << Err::int1_t(id())
      );

      Node* p = np.get();
      //ignore erased_
      const std::size_t n = sz(id);
      if( n < v_.size() ) {
         v_.replace(n, np);
      } else {
         //v_.resize(n,0) does not compile for some reason
         while( n > v_.size() ) v_.push_back(0);
         v_.push_back(np);
      }
      m_.emplace(p, id);
   }

};

}//namespace detail
}//namespace owlcpp
#endif /* MAP_NODE_BASE_HPP_ */
