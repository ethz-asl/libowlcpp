/** @file "/owlcpp/include/owlcpp/rdf/map_node.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_BASE_HPP_
#define MAP_NODE_BASE_HPP_
#include <string>
#include <vector>
#include <memory>
#include "boost/assert.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/ptr_container/indirect_fun.hpp"
#include "boost/unordered_map.hpp"
#include "boost/foreach.hpp"
#include "boost/concept_check.hpp"

#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/detail/iterator_member_pair.hpp"
#include "owlcpp/detail/datatype_impl.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

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
   typedef std::auto_ptr<node_type> ptr_t;
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
   typedef std::pair<Node const*,Node_id> map_element_t;

public:
   typedef Iterator_member_pair<map_citer_t, Node_id const, 2> iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   Map_node() {}

   Map_node(Map_node const& mn)
   : vid_(mn.vid_.size()), map_(), erased_(mn.erased_)
   {
      copy(mn);
   }

   Map_node& operator=(Map_node const& mn) {
      if( this != &mn) {
         clear();
         vid_.reserve(mn.vid_.size());
         copy(mn);
         erased_ = mn.erased_;
      }
      return *this;
   }

   std::size_t size() const { return map_.size(); }
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}
   bool empty() const {return map_.empty();}

   Node const& operator[](const Node_id id) const {
      const std::size_t n = vpos(id);
      BOOST_ASSERT( n < vid_.size() );
      BOOST_ASSERT( ! vid_.is_null(n) );
      return vid_[n];
   }

   Node const& at(const Node_id id) const {
      if( Node const* node = find(id) ) return *node;
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid node ID")
               << Err::int1_t(id())
      );
   }

   Node const* find(const Node_id id) const {
      if(
               id < detail::min_node_id() ||
               id() >= vid_.size() + detail::min_node_id()()
      ) return 0;
      return  &vid_[vpos(id)];
   }

   Node_id const* find(Node const& node) const {
      const map_citer_t i = map_.find(&node);
      return i == map_.end() ? 0 : &i->second;
   }

   Node_id const* find_iri(const Ns_id ns, std::string const& val) const {
      return find(Node_iri(ns, val));
   }

   Node_id const* find_literal(
            std::string const& val, const Node_id dt, std::string const& lang
   ) const {
      switch(internal_type_id(dt)) {
      case detail::Bool_tid:
         return find(Node_bool(val, dt));
      case detail::Int_tid:
         return find(Node_int(val, dt));
      case detail::Unsigned_tid:
         return find(Node_unsigned(val, dt));
      case detail::Double_tid:
         return find(Node_double(val, dt));
      case detail::String_tid:
      case detail::Unknown_tid:
         return find( Node_string(val, dt, lang) );
      case detail::Empty_tid:
      default:
         return find( Node_string(val, lang) );
      }
   }

   Node_id const* find_blank(const unsigned n, const Doc_id doc) const {
      return find(Node_blank(n, doc));
   }

   /** insert a clone of the %node */
   Node_id insert(Node const& node) {
      if( Node_id const* id = find(node) ) return *id;
      Node* np = node.clone();

      //make new ID
      if( erased_.empty() ) {
         const Node_id id = nid(vid_.size());
         vid_.push_back(np);
         map_.emplace(np, id);
         return id;
      }
      const Node_id id = erased_.back();
      erased_.pop_back();
      const std::size_t n = vpos(id);
      BOOST_ASSERT(vid_.is_null(n));
      vid_.replace(n, np);
      map_.emplace(np, id);
      return id;
   }

   Node_id insert_iri(const Ns_id ns, std::string const& val) {
      return insert(Node_iri(ns,val));
   }

   Node_id insert_literal(
            std::string const& val,
            const Node_id dt,
            std::string const& lang = ""
   ) {
      switch(internal_type_id(dt)) {
      case detail::Bool_tid:
         return insert( Node_bool(val, dt) );
      case detail::Int_tid:
         return insert( Node_int(val, dt) );
      case detail::Unsigned_tid:
         return insert( Node_unsigned(val, dt) );
      case detail::Double_tid:
         return insert( Node_double(val, dt) );
      case detail::String_tid:
      case detail::Unknown_tid:
         return insert( Node_string(val, dt, lang) );
      case detail::Empty_tid:
      default:
         return insert( Node_string(val, lang) );
      }
   }

   Node_id insert_blank(const unsigned n, const Doc_id doc) {
      return insert(Node_blank(n, doc));
   }

   std::auto_ptr<Node> remove(const Node_id id) {
      BOOST_ASSERT(find(id));
      const std::size_t n = map_.erase(&get(id));
      boost::ignore_unused_variable_warning(n);
      BOOST_ASSERT(n);

      erased_.push_back(id);
      return ptr_t( vid_.replace(vpos(id), 0).release() );
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

   std::size_t vpos(const Node_id id) const {
      BOOST_ASSERT(id >= detail::min_node_id());
      return id() - detail::min_node_id()();
   }

   Node_id nid(const std::size_t n) const {return Node_id(n + detail::min_node_id()());}

   Node const& get(const Node_id id) const {return vid_[vpos(id)];}

   void check_insert(const Node_id id, ptr_t np) {
      Node_id const* id0 = find(*np);
      if( id0 ) {
         if( *id0 == id ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("node already exists")
                  << Err::node_id_t(*id0)
                  << Err::str1_t(to_string(*np))
         );
      }
      if( find(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("node ID is reserved")
               << Err::int1_t(id())
      );
      insert(id, np.release());
   }

   void insert(const Node_id id, Node* np) {
      const std::size_t n = vpos(id);
      if( n < vid_.size() ) {
         vid_.replace(n, np);
      } else {
         vid_.reserve(n);
         for( ; vid_.size() < n; vid_.push_back(0));
         vid_.push_back(np);
      }
      map_.emplace(np, id);
   }

   void copy(Map_node const& mn) {
      BOOST_FOREACH(map_element_t const& p, mn.map_) {
         Node* np = p.first->clone();
         insert(p.second, np);
      }
   }

};

}//namespace owlcpp
#endif /* MAP_NODE_BASE_HPP_ */
