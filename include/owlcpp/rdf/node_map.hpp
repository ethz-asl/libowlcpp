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

#include "owlcpp/node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/terms/iri_tags.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{

class No_nodes {
   static Node const& empty() {
      static const Node node((terms::T_empty_())); //extra brackets to distinguish from function
      return node;
   }
public:
   Node_id max_id() const {return terms::T_empty_::id();}
   bool have(const Node_id nid) const {return nid == terms::T_empty_::id();}
   bool have(const Ns_id iid) const {return false;}
   Node const& operator[](const Node_id id) const {return at(id);}
   Node const& at(const Node_id nid) const {
      if(nid != terms::T_empty_::id()) BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("unknown node ID")
               << Rdf_err::int1_t(nid())
      );
      return empty();
   }
   Node_map_base::ns_range find(const Ns_id iid) const {return Node_map_base::ns_range();}
   Node_map_base::node_range find(Node const& node) const {return Node_map_base::node_range();}
};

}//namespace detail

/**@brief Store RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
template<class StdNodes = detail::No_nodes> class Node_map {

   typedef boost::unordered_map<Node_id,Node_id> datatypes_t;
   typedef datatypes_t::const_iterator datatype_iter_t;

   typedef boost::unordered_map<Node_id,const std::string> langs_t;
   typedef langs_t::const_iterator lang_iter_t;

   typedef boost::unordered_map<Node_id,Doc_id> docs_t;
   typedef docs_t::const_iterator doc_iter_t;

public:
   typedef Node_map_base::const_iterator const_iterator;
   typedef Node_map_base::iterator iterator;
   typedef Node_map_base::range range;
   typedef Node_map_base::node_iterator node_iterator;
   typedef Node_map_base::node_range node_range;
   typedef Node_map_base::ns_iterator ns_iterator;
   typedef Node_map_base::ns_range ns_range;
   typedef Rdf_err Err;

   Node_map()
   : std_nodes_(),
     tracker_(std_nodes_.max_id())
   {}

   std::size_t size() const {return nodes_.size();}
   const_iterator begin() const {return nodes_.begin();}
   const_iterator end() const {return nodes_.end();}

   Node const& operator[](const Node_id id) const {
      return std_nodes_.have(id) ? std_nodes_[id] : nodes_[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      return std_nodes_.have(id) ? std_nodes_.at(id) : nodes_.at(id);
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
   ns_range find(const Ns_id iid) const {
      return std_nodes_.have(iid) ? std_nodes_.find(iid) : nodes_.find(iid);
   }

   node_range find(Node const& node) const {
      return std_nodes_.have(node.ns_id()) ? std_nodes_.find(node) : nodes_.find(node);
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

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      if( nsid == terms::N_empty::id() )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("empty namespace for IRI node")
                  << Err::str1_t(name)
         );
      if( nsid == terms::N_blank::id() )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("blank namespace for IRI node")
                  << Err::str1_t("_" + name)
         );
      const Node node(nsid, name);
      if( std_nodes_.have(nsid) ) {
         const node_range r = std_nodes_.find(node);
         if( r ) return r.front();
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("new term cannot be inserted into standard namespace")
                  << typename Err::str1_t( name )
         );
      }
      const node_range nr = nodes_.find(node);
      if( nr ) return nr.front();
      const Node_id id = tracker_.get();
      nodes_.insert(id, node);
      return id;
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
      const Node_id id = tracker_.get();
      nodes_.insert(id, node);
      docs_.insert(std::make_pair(id, did));
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
         Node_id const* dtp = datatype(id);
         const Node_id dt = dtp ? *dtp : terms::T_empty_::id();
         if( dtype == dt && lang == language(id) ) return id;
      }
      const Node_id id = tracker_.get();
      nodes_.insert(id, node);
      if( dtype != terms::T_empty_::id() ) dtypes_.insert(std::make_pair(id, dtype));
      if( ! lang.empty() ) langs_.insert(std::make_pair(id, lang));
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
      nodes_.remove(id);
      tracker_.push(id);
   }

private:
   const StdNodes std_nodes_;
   detail::Id_tracker<Node_id> tracker_;
   Node_map_base nodes_;
   docs_t docs_;
   datatypes_t dtypes_;
   langs_t langs_;
};

}//namespace owlcpp
#endif /* NODE_MAP_HPP_ */
