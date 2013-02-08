/** @file "/owlcpp/include/owlcpp/rdf/map_node_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_CRTPB_HPP_
#define MAP_NODE_CRTPB_HPP_

#include "owlcpp/detail/map_traits.hpp"

namespace owlcpp{

/**Expose IRI node map interface.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_node_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_node_type map_node_type;
   typedef typename traits::node_type node_type;

   map_node_type const& _map_node() const {
      return static_cast<Super const&>(*this).map_node_;
   }

   map_node_type& _map_node() {
      return static_cast<Super&>(*this).map_node_;
   }

public:

   node_type const& operator[](const Node_id id) const {
      return _map_node()[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   node_type const& at(const Node_id id) const {
      return _map_node().at(id);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_node_iri(const Ns_id nsid, std::string const& name) {
      BOOST_ASSERT(
               static_cast<Super const&>(*this).find(nsid) &&
               "invalid namespace ID"
      );
      typedef typename Super::Err Err;
      if( is_blank(nsid) ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("blank namespace for IRI node")
                  << typename Err::str1_t(name)
         );
      }
      return _map_node().insert_iri(nsid, name);
   }

   Node_id const* find_node_iri(const Ns_id nsid, std::string const& name) const {
      BOOST_ASSERT(
               static_cast<Super const&>(*this).find(nsid) &&
               "invalid namespace ID"
      );
      return _map_node().find_iri(nsid, name);
   }

};

}//namespace owlcpp
#endif /* MAP_NODE_CRTPB_HPP_ */
