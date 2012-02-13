/** @file "/owlcpp/include/owlcpp/rdf/node_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_HPP_
#define NODE_MAP_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/unordered_map.hpp"
#include "boost/range.hpp"

#include "owlcpp/node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{

class Node_tag_inserter;

}

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class OWLCPP_RDF_DECL Node_map {
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

   friend class detail::Node_tag_inserter;

   typedef boost::unordered_map<Node_id,Node_id> datatypes_t;
   typedef datatypes_t::const_iterator datatype_iter_t;

   typedef boost::unordered_map<Node_id,const std::string> langs_t;
   typedef langs_t::const_iterator lang_iter_t;

   typedef boost::unordered_map<Node_id,Doc_id> docs_t;
   typedef docs_t::const_iterator doc_iter_t;

public:
   typedef nodes_t::iterator iterator;
   typedef iterator const_iterator;
   typedef iri_index_t::iterator iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   struct Err : public base_exception {};
   Node_map();
   std::size_t size() const {return nodes_.size();}

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(nodes_.get<id_tag>().find(id) != nodes_.get<id_tag>().end());
      return nodes_.get<id_tag>().find(id)->second;
   }

   /**@brief Find the datatype of the literal node
    @param id literal node's ID
    @return pointer to the datatype node's ID for the literal node or
    NULL if the node is not literal or no datatype is defined for it.
   */
   Node_id const* datatype(const Node_id id) const {
      const datatype_iter_t i = dtypes_.find(id);
      if( i == dtypes_.end() ) return 0;
      return &i->second;
   }

   /**@brief Find the language of the literal node
    @param id literal node's ID
    @return language tag string for the literal node or
    "" if the node is not literal or no language is defined for it.
    @details The tag string format MAY be according to RFC 5646
    (http://tools.ietf.org/html/rfc5646).
    This is not, however, currently enforced by the library.
   */
   std::string language(const Node_id id) const {
      const lang_iter_t i = langs_.find(id);
      if( i == langs_.end() ) return "";
      return i->second;
   }

   /**@brief Find nodes in a given namespace
    @param iid namespace IRI ID
    @return iterator range for nodes in namespace iid
    @details For example:
    @code
    find(terms::N_owl::id()) //returns range of all nodes in OWL namespace
    find(terms::N_empty::id()) //returns range of all literal nodes
    find(terms::N_blank::id()) //returns range of all blank nodes
    @endcode
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

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id);

   void remove(Node const& node);
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      node_index_t const& n_index = nodes_.get<node_tag>();
      const Node node(nsid, name);
      const node_iter_t n_iter = n_index.find(node);
      if( n_iter != n_index.end() ) return n_iter->first;
      const Node_id id = tracker_.get();
      insert(id, node);
      return id;
   }

   /**@brief Insert blank node
    @param did document ID
    @param name blank node name (MUST be unique within the document)
    @return node ID
   */
   Node_id insert_blank(const Doc_id did, std::string const& name) {
      node_index_t const& n_index = nodes_.get<node_tag>();
      const Node node(terms::N_blank::id(), name);
      const node_iter_t n_iter = n_index.find(node);
      if( n_iter != n_index.end() ) {
         const Node_id id = n_iter->first;
         BOOST_ASSERT(docs_.find(id) != docs_.end());
         if( docs_[id] == did ) return id;
      }
      const Node_id id = insert_iri(terms::N_blank::id(), name);
      docs_.insert(std::make_pair(id, did));
      return id;
   }

   /**@brief Insert literal node
    @param value
    @param datatype
    @param lang language tag string for the literal node or
    "" if the language is not defined.
    The tag string format SHOULD be according to RFC 5646
    (http://tools.ietf.org/html/rfc5646).
    This is not, however, currently enforced by the library.
    @return node ID
   */
   Node_id insert_literal(
            std::string const& value,
            const Node_id datatype = terms::T_empty_::id(),
            std::string const& lang = ""
   );

   /**
    @param id ID of a blank node
    @return ID of the document to which the blank node belongs
    @throw Err if @b id is not for a blank node
   */
   Doc_id blank_node_doc(const Node_id id) const {
      //BOOST_ASSERT(docs_.find(id) != docs_.end());
      const doc_iter_t i = docs_.find(id);
      if( i == docs_.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("document unknown")
               << Err::int1_t(id())
      );
      return i->second;
   }

private:
   detail::Id_tracker<Node_id> tracker_;
   nodes_t nodes_;
   docs_t docs_;
   datatypes_t dtypes_;
   langs_t langs_;

   void insert(const Node_id id, Node const& node) {
      BOOST_ASSERT(
               nodes_.get<id_tag>().find(id) == nodes_.get<id_tag>().end()
      );
      BOOST_ASSERT(
               nodes_.get<node_tag>().find(node) == nodes_.get<node_tag>().end()
      );
      nodes_.insert( std::make_pair(id, node) );
      tracker_.ensure_min(id);
   }

   Node_id insert_literal_private(
            Node const& node,
            const Node_id dt,
            std::string const& lang
   ) {
      const Node_id id = tracker_.get();
      insert(id, node);
      if( dt != terms::T_empty_::id() ) dtypes_.insert(std::make_pair(id, dt));
      if( ! lang.empty() ) langs_.insert(std::make_pair(id, lang));
      return id;
   }

};

}//namespace owlcpp
#endif /* NODE_MAP_HPP_ */
