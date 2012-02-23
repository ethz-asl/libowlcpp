/** @file "/owlcpp/include/owlcpp/rdf/node_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_HPP_
#define NODE_MAP_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/node_map_iri.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_map {
   typedef boost::unordered_map<Node_id,Node_id> datatypes_t;
   typedef datatypes_t::const_iterator datatype_iter_t;

   typedef boost::unordered_map<Node_id,const std::string> langs_t;
   typedef langs_t::const_iterator lang_iter_t;

   typedef boost::unordered_map<Node_id,Doc_id> docs_t;
   typedef docs_t::const_iterator doc_iter_t;

public:
   typedef Node_map_iri::const_iterator const_iterator;
   typedef Node_map_iri::iterator iterator;
   typedef Node_map_iri::range range;
   typedef Node_map_iri::node_iterator node_iterator;
   typedef Node_map_iri::node_range node_range;
   typedef Node_map_iri::ns_iterator ns_iterator;
   typedef Node_map_iri::ns_range ns_range;
   typedef Rdf_err Err;

   Node_map(Node_map_std const& snodes = Node_map_std::get(Nodes_none()))
   : nodes_(snodes)
   {}

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   Node const& operator[](const Node_id id) const {return nodes_[id];}

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   Node const& at(const Node_id id) const {return nodes_.at(id);}

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
   ns_range find(const Ns_id iid) const {return nodes_.find(iid);}

   node_range find(Node const& node) const {return nodes_.find(node);}

   /**@brief Find the datatype of the literal node
    @param id literal node's ID
    @return pointer to the datatype node's ID for the literal node or
    NULL if the node is not literal or no datatype is defined for it.
   */
   Node_id datatype(const Node_id id) const {
      const datatype_iter_t i = dtypes_.find(id);
      if( i == dtypes_.end() ) return terms::T_empty_::id();
      return i->second;
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

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      return nodes_.insert_iri(nsid, name);
   }

   /**@brief Insert blank node
    @param did document ID
    @param name blank node name (MUST be unique within the document)
    @return node ID
   */
   Node_id insert_blank(const Doc_id did, std::string const& name) {
      const Node node(terms::N_blank::id(), name);
      BOOST_FOREACH(const Node_id id, find(node)) {
         if( blank_node_doc(id) == did ) return id;
      }
      const Node_id id = nodes_.insert(terms::N_blank::id(), name);
      docs_.emplace(id, did);
      return id;
   }

   /**@brief Insert literal node
    @param value
    @param dtype datatype node ID
    @param lang language tag string for the literal node or
    "" if the language is not defined.
    The tag string format SHOULD be according to RFC 5646
    (http://tools.ietf.org/html/rfc5646).
    This is not, however, currently enforced by the library.
    @return node ID
   */
   Node_id insert_literal(
            std::string const& value,
            const Node_id dtype = terms::T_empty_::id(),
            std::string const& lang = ""
   ) {
      const Node node(terms::N_empty::id(), value);
      BOOST_FOREACH(const Node_id id, find(node)) {
         const Node_id dtype0 = datatype(id);
         if( dtype == dtype0 && lang == language(id) ) return id;
      }
      const Node_id id = nodes_.insert(terms::N_empty::id(), value);
      if( ! is_empty(dtype) ) dtypes_.emplace(id, dtype);
      if( ! lang.empty() ) langs_.emplace(id, lang);
      return id;

   }

   /**
    @param id ID of a blank node
    @return ID of the document to which the blank node belongs
    @throw Rdf_err if @b id is not for a blank node
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

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {
      Node const& node = nodes_[id];
      if( node.ns_id() == terms::N_blank::id() ) docs_.erase(id);
      else if( node.ns_id() == terms::N_empty::id() ) {
         dtypes_.erase(id);
         langs_.erase(id);
      }
      nodes_.remove(id);
   }

private:
   Node_map_iri nodes_;
   docs_t docs_;
   datatypes_t dtypes_;
   langs_t langs_;
};
}//namespace owlcpp

#include "owlcpp/rdf/node_iter_crtp.hpp"
#include "owlcpp/rdf/std_nodes_crtp.hpp"

namespace owlcpp{

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Node_map1 :
   public Std_nodes_crtp<Node_map1>,
   public Node_iter_crtp<Node_map1>
   {
   friend class Std_nodes_crtp<Node_map1>;
   friend class Node_iter_crtp<Node_map1>;

   Node_map_base& base_map() {return nodes_;}
   Node_map_base const& base_map() const {return nodes_;}

public:
   Node_map1(Node_map_std const& snodes = Node_map_std::get(Nodes_none()))
   : Std_nodes_crtp<Node_map1>(snodes)
     {}

private:
   detail::Id_tracker<Node_id> tracker_;
   Node_map_base nodes_;

};

}//namespace owlcpp
#endif /* NODE_MAP_HPP_ */
