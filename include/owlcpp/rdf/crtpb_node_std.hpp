/** @file "/owlcpp/include/owlcpp/rdf/crtpb_node_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CRTPB_NODE_STD_HPP_
#define CRTPB_NODE_STD_HPP_
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**Enable interaction between a mutable map of user nodes and constant map
of standard IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Crtpb_node_std {
   typedef typename Map_traits<Super>::map_node_std_t map_node_std_t;
   typedef typename Map_traits<Super>::map_node_t map_node_t;
   typedef typename map_node_t::node_type node_type;

   map_node_std_t const& smap() const {
      return static_cast<Super const&>(*this).snode_;
   }

   map_node_t const& nodes() const {
      return static_cast<Super const&>(*this).nodes();
   }

   map_node_t& nodes() {
      return static_cast<Super&>(*this).node_;
   }

public:

   node_type const& operator[](const Node_id id) const {
      return id < smap().node_id_next() ? smap()[id] : nodes()[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   node_type const& at(const Node_id id) const {
      return id < smap().node_id_next() ? smap().at(id) : nodes().at(id);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_node_iri(const Ns_id nsid, std::string const& name) {
      typedef typename Super::Err Err;
      if( is_blank(nsid) ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("blank namespace for IRI node")
                  << typename Err::str1_t(name)
         );
      }
      if( Node_id const*const nid = smap().find(nsid, name) ) return *nid;

      if( smap().is_standard(nsid) ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("new term cannot be inserted into standard namespace")
                  << typename Err::str1_t( name )
                  << typename Err::str2_t( smap().at(nsid) )
         );
      }
      return nodes().insert_iri(nsid, name);
   }
};

}//namespace owlcpp
#endif /* CRTPB_NODE_STD_HPP_ */
