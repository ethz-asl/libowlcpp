/** @file "/owlcpp/include/owlcpp/rdf/nodes_literal_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODES_LITERAL_CRTPB_HPP_
#define NODES_LITERAL_CRTPB_HPP_
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/node_map_base.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
template<class Super> class Nodes_literal_crtpb {
   typedef boost::unordered_map<Node_id,Node_id> datatypes_t;
   typedef datatypes_t::const_iterator datatype_iter_t;

   typedef boost::unordered_map<Node_id,const std::string> langs_t;
   typedef langs_t::const_iterator lang_iter_t;

public:

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
      Node_id const*const nid1 = find_literal(value, dtype, lang);
      if( nid1 ) return *nid1;
      Super& self = static_cast<Super&>(*this);
      const Node_id nid2 = self.insert_new(terms::N_empty::id(), value);
      if( ! is_empty(dtype) ) dtypes_.emplace(nid2, dtype);
      if( ! lang.empty() ) langs_.emplace(nid2, lang);
      return nid2;
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
      const lang_iter_t li = langs_.find(id);
      return li == langs_.end() ? "" : li->second;
   }

   /**@brief Find literal node datatype
    @param id literal node's ID
    @return datatype node ID for the literal node or empty node ID
    if the node is not literal or datatype is not defined for it.
   */
   Node_id datatype(const Node_id id) const {
      const datatype_iter_t di = dtypes_.find(id);
      return di == dtypes_.end() ? terms::T_empty_::id() : di->second;
   }

   Node_id const* find_literal(
            std::string const& value,
            const Node_id dtype = terms::T_empty_::id(),
            std::string const& lang = ""
   ) const {
      Node_map_base const& nmb = static_cast<Super const&>(*this).node_map();
      BOOST_FOREACH(Node_id const& id, nmb.find(Node(terms::N_empty::id(), value))) {
         if( dtype != datatype(id) ) continue;
         if( lang != language(id) ) continue;
         return &id;
      }
      return 0;
   }

protected:
   void remove_literal(const Node_id nid) {
      dtypes_.erase(nid);
      langs_.erase(nid);
   }

   void clear_literal() {
      dtypes_.clear();
      langs_.clear();
   }

private:
   datatypes_t dtypes_;
   langs_t langs_;

};

}//namespace owlcpp
#endif /* NODES_LITERAL_CRTPB_HPP_ */
