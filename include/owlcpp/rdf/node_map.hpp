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
#include "owlcpp/rdf/doc_id.hpp"
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
   typedef Node_id id_type;
   typedef std::pair<id_type, Node> value_t;
private:
   static Ns_id ns_id(value_t const& v) {return v.second.ns_id();}

   typedef boost::multi_index_container<
         value_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  value_t, id_type, &value_t::first
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
      > store_t;
   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef store_t::index<node_tag>::type node_index_t;
   typedef node_index_t::iterator node_iter_t;
   typedef store_t::index<iri_tag>::type iri_index_t;

   friend class detail::Node_tag_inserter;

   typedef boost::unordered_map<id_type,id_type> datatype_stor_t;
   typedef datatype_stor_t::const_iterator datatype_iter_t;

   typedef boost::unordered_map<id_type,const std::string> lang_stor_t;
   typedef lang_stor_t::const_iterator lang_iter_t;

public:
   typedef store_t::iterator iterator;
   typedef iterator const_iterator;
   typedef iri_index_t::iterator iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   struct Err : public base_exception {};
   Node_map();
   std::size_t size() const {return store_.size();}

   Node const& operator[](const Node_id id) const {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(id)->second;
   }

   /**@brief Find the datatype of the literal node
    @param id literal node's ID
    @return pointer to the datatype node's ID for the literal node or
    NULL if the node is not literal or no datatype is defined for it.
   */
   Node_id const* datatype(const Node_id id) const {
      const datatype_iter_t i = datatype_.find(id);
      if( i == datatype_.end() ) return 0;
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
      const lang_iter_t i = lang_.find(id);
      if( i == lang_.end() ) return "";
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
               store_.get<iri_tag>().equal_range(iid)
      );
   }

   Node_id const* find(Node const& node) const {
      node_index_t const& node_index = store_.get<node_tag>();
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
      id_index_t const& index = store_.get<id_tag>();
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
   void remove(const id_type id);

   void remove(Node const& node);
   const_iterator begin() const {return store_.begin();}
   const_iterator end() const {return store_.end();}

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   id_type insert_iri(const Ns_id nsid, std::string const& name) {
      node_index_t const& n_index = store_.get<node_tag>();
      const Node node(nsid, name);
      const node_iter_t n_iter = n_index.find(node);
      if( n_iter != n_index.end() ) return n_iter->first;
      const id_type id = tracker_.get();
      insert(id, node);
      return id;
   }

   /**@brief Insert blank node
    @param name
    @return node ID
   */
   id_type insert_blank(std::string const& name, const Doc_id doc) {
      return insert_iri(terms::N_blank::id(), name);
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
   id_type insert_literal(
            std::string const& value,
            const id_type datatype = terms::T_empty_::id(),
            std::string const& lang = ""
   );

private:
   detail::Id_tracker<id_type> tracker_;
   store_t store_;
   datatype_stor_t datatype_;
   lang_stor_t lang_;

   void insert(const Node_id id, Node const& node) {
      BOOST_ASSERT(
               store_.get<id_tag>().find(id) == store_.get<id_tag>().end()
      );
      BOOST_ASSERT(
               store_.get<node_tag>().find(node) == store_.get<node_tag>().end()
      );
      store_.insert( std::make_pair(id, node) );
      tracker_.ensure_min(id);
   }

   id_type insert_literal_private(
            Node const& node,
            const id_type dt,
            std::string const& lang
   ) {
      const id_type id = tracker_.get();
      insert(id, node);
      if( dt != terms::T_empty_::id() ) datatype_.insert(std::make_pair(id, dt));
      if( ! lang.empty() ) lang_.insert(std::make_pair(id, lang));
      return id;
   }

};

/** Copy nodes from one map to another taking into account changed IRI IDs
and insert pairs of old and new node IDs into @b id_map

template<class NodeMap1, class NodeMap2, class IriIdMap, class NodeIdMap> inline void
copy_nodes(
         NodeMap1 const& nm1,
         NodeMap2& nm2,
         IriIdMap const& iid_map,
         NodeIdMap& nid_map
) {
   typedef typename NodeMap1::value_t pair_t;
   typedef typename NodeMap2::id_type id_t;
   BOOST_FOREACH(pair_t const& p, nm1) {
      Node const& node = p.second;
      switch (node.ns_id()()) {
         case terms::N_blank::index: {

         }
            break;
         default:
            break;
      }
      const id_t id = nm2.insert_iri(iid_map[node.ns_id()], node.value_str());

   }
}
*******************************************************************************/

}//namespace owlcpp
#endif /* NODE_MAP_HPP_ */
