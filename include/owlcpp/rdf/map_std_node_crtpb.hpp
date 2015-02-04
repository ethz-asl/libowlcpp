/** @file "/owlcpp/include/owlcpp/rdf/map_std_node_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_STD_NODE_CRTPB_HPP_
#define MAP_STD_NODE_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/concept/assert.hpp"

#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**Enable interaction between a mutable maps of namespaces and nodes and
immutable map of standard IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_std_node_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_std_type map_std_type;
   typedef typename traits::map_node_type map_node_type;

   map_std_type const& _map_std() const {
      return static_cast<Super const&>(*this).map_std_;
   }

   map_node_type const& _map_node() const {
      return static_cast<Super const&>(*this).map_node_;
   }

   map_node_type& _map_node() {
      return static_cast<Super&>(*this).map_node_;
   }

public:
   typedef typename traits::node_type node_type;

   bool is_standard(const Node_id nid) const {return _map_std().is_standard(nid);}

   node_type const& operator[](const Node_id id) const {
      return _map_std().is_standard(id) ? _map_std()[id] : _map_node()[id];
   }

   /**
    @param id node ID
    @return node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   node_type const& at(const Node_id id) const {
      return _map_std().is_standard(id) ? _map_std().at(id) : _map_node().at(id);
   }

   /**
    @param id node ID
    @return pointer to node object with specified ID or NULL if ID is invalid
   */
   node_type const* find(const Node_id id) const {
      return _map_std().is_standard(id) ?
               _map_std().find(id) :
               _map_node().find(id)
               ;
   }

   Node_id const* find_node_iri(const Ns_id nsid, std::string const& name) const {
      BOOST_ASSERT(
               static_cast<Super const&>(*this).find(nsid) &&
               "invalid namespace ID"
      );
      if( Node_id const*const nid = _map_std().find(nsid, name) ) return nid;
      return _map_node().find_iri(nsid, name);
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
      if( Node_id const*const nid = _map_std().find(nsid, name) ) return *nid;

      if( _map_std().is_standard(nsid) ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("unknown term in standard namespace")
                  << typename Err::str1_t( name )
                  << typename Err::str2_t( _map_std().at(nsid).str() )
         );
      }
      return _map_node().insert_iri(nsid, name);
   }
};

}//namespace owlcpp
#endif /* MAP_STD_NODE_CRTPB_HPP_ */
