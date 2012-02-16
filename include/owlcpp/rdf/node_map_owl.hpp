/** @file "/owlcpp/include/owlcpp/rdf/node_map_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_OWL_HPP_
#define NODE_MAP_OWL_HPP_
#include "owlcpp/rdf/owl_terms.hpp"
#include "owlcpp/rdf/node_map.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Node_map_owl {
public:
   typedef Node_map::iterator iterator;
   typedef Node_map::const_iterator const_iterator;
   typedef Node_map::range range;
   typedef Node_map::iri_iterator iri_iterator;
   typedef Node_map::iri_range iri_range;

   struct Err : public base_exception {};

   Node_map_owl() : map_(Owl_terms::max_id()) {}

   std::size_t size() const {return map_.size();}
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}

   Node const& operator[](const Node_id id) const {
      if( Owl_terms::is_owl(id) ) return Owl_terms::get()[id];
      return map_[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Node_map::Err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      if( Owl_terms::is_owl(id) ) return Owl_terms::get().at(id);
      return map_.at(id);
   }

   /**@brief Find the datatype of the literal node
    @param id literal node's ID
    @return pointer to the datatype node's ID for the literal node or
    NULL if the node is not literal or no datatype is defined for it.
   */
   Node_id const* datatype(const Node_id id) const {return map_.datatype(id);}

   /**@brief Find the language of the literal node
    @param id literal node's ID
    @return language tag string for the literal node or
    "" if the node is not literal or no language is defined for it.
    @details The tag string format MAY be according to RFC 5646
    (http://tools.ietf.org/html/rfc5646).
    This is not, however, currently enforced by the library.
   */
   std::string language(const Node_id id) const {return map_.language(id);}

   /**@brief Find nodes in a given namespace
    @param iid namespace IRI ID
    @return iterator range for nodes in namespace iid
    @details For example:
    @code
    find(terms::N_empty::id()) //returns range of all literal nodes
    find(terms::N_blank::id()) //returns range of all blank nodes
    @endcode
   */
   iri_range find(const Ns_id iid) const {return map_.find(iid);}

   Node_id const* find(Node const& node) const {
      if( Owl_terms::is_owl(node.ns_id()) ) return Owl_terms::get().find(node);
      return map_.find(node);
   }

   /**@brief Remove node with specified ID
    @param id node ID
    @details
    If node with ID @b id does not exist, behavior is undefined.
   */
   void remove(const Node_id id) {if( ! Owl_terms::is_owl(id) ) map_.remove(id);}

   void remove(Node const& node) {
      if( ! Owl_terms::is_owl(node.ns_id()) ) map_.remove(node);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      if( Owl_terms::is_owl(nsid) ) {
         Node_id const* const id = Owl_terms::get().find(Node(nsid, name));
         if( id ) return *id;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("new term cannot be inserted into standard namespace")
                  << typename Err::str1_t( name )
         );

      }
      return map_.insert_iri(nsid, name);
   }

   /**@brief Insert blank node
    @param did document ID
    @param name blank node name (MUST be unique within the document)
    @return node ID
   */
   Node_id insert_blank(const Doc_id did, std::string const& name) {
      return map_.insert_blank(did, name);
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
   ) {
      return map_.insert_literal(value, datatype, lang);
   }

   /**
    @param id ID of a blank node
    @return ID of the document to which the blank node belongs
    @throw Err if @b id is not for a blank node
   */
   Doc_id blank_node_doc(const Node_id id) const {return map_.blank_node_doc(id);}

private:
   Node_map map_;
};

}//namespace owlcpp
#endif /* NODE_MAP_OWL_HPP_ */
