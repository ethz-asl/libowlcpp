/** @file "/owlcpp/include/owlcpp/rdf/map_node_literal_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_LITERAL_CRTPB_HPP_
#define MAP_NODE_LITERAL_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"

#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/store_concepts.hpp"

namespace owlcpp{

/**Enable interface for literal nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_node_literal_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_node_type map_node_type;

   map_node_type const& _map_node() const {
      return static_cast<Super const&>(*this).map_node_;
   }

   map_node_type& _map_node() {
      return static_cast<Super&>(*this).map_node_;
   }

public:

   /** @brief find literal node
    @param value
    @param dtype
    @param lang
    */
   Node_id const* find_literal(
            std::string const& value,
            const Node_id dtype = terms::T_empty_::id(),
            std::string const& lang = ""
   ) const {
      return _map_node().find_literal(value, dtype, lang);
   }

   /**@brief Insert literal node
    @param value
    @param dt_id datatype node ID
    @param lang language tag string for the literal node or
    "" if the language is not defined.
    The tag string format SHOULD be according to RFC 5646
    (http://tools.ietf.org/html/rfc5646).
    This is not, however, currently enforced by the library.
    @return node ID
   */
   Node_id insert_literal(
            std::string const& value,
            const Node_id dt_id = terms::T_empty_::id(),
            std::string const& lang = ""
   ) {
      return _map_node().insert_literal(value, dt_id, lang);
   }

   /**@brief Insert literal node
    @param value
    @param dt_iri datatype node ID
    @param lang language tag string for the literal node or
    "" if the language is not defined.
    The tag string format SHOULD be according to RFC 5646
    (http://tools.ietf.org/html/rfc5646).
    This is not, however, currently enforced by the library.
    @return node ID
   */
   Node_id insert_literal(
            std::string const& value,
            std::string const& dt_iri,
            std::string const& lang = ""
   ) {
      const Node_id dt_id = static_cast<Super&>(*this).insert_node_iri(dt_iri);
      return _map_node().insert_literal(value, dt_id, lang);
   }
};

}//namespace owlcpp
#endif /* MAP_NODE_LITERAL_CRTPB_HPP_ */
