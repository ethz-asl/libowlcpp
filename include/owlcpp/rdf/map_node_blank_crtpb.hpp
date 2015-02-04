/** @file "/owlcpp/include/owlcpp/rdf/map_node_blank_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_BLANK_CRTPB_HPP_
#define MAP_NODE_BLANK_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"

#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/rdf/store_concepts.hpp"

namespace owlcpp{

/**Enable interface for blank nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_node_blank_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_node_type map_node_type;

   map_node_type const& _map_node() const {
      return static_cast<Super const&>(*this).map_node_;
   }

   map_node_type& _map_node() {
      return static_cast<Super&>(*this).map_node_;
   }

public:

   Node_id const* find_blank(const unsigned n, const Doc_id did) const {
      BOOST_CONCEPT_ASSERT((Doc_store<Super>));
      BOOST_ASSERT(static_cast<Super const&>(*this).find(did));
      return _map_node().find_blank(n, did);
   }

   /**@brief Insert blank node
    @param n blank node index (MUST be unique within the document)
    @param did document ID
    @return node ID
   */
   Node_id insert_blank(const unsigned n, const Doc_id did) {
      BOOST_CONCEPT_ASSERT((Doc_store<Super>));
      BOOST_ASSERT(static_cast<Super const&>(*this).find(did));
      return _map_node().insert_blank(n, did);
   }
};

}//namespace owlcpp
#endif /* MAP_NODE_BLANK_CRTPB_HPP_ */
