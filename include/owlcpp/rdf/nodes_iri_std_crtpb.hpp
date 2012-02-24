/** @file "/owlcpp/include/owlcpp/rdf/nodes_iri_std_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODES_IRI_STD_CRTPB_HPP_
#define NODES_IRI_STD_CRTPB_HPP_
#include "owlcpp/rdf/node_map_std.hpp"
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/rdf/std_nodes.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
template<class Super> class Nodes_iri_std_crtpb {
public:
   typedef Node_map_base::ns_iterator ns_iterator;
   typedef Node_map_base::ns_range ns_range;
   typedef Node_map_base::node_iterator node_iterator;
   typedef Node_map_base::node_range node_range;

   Nodes_iri_std_crtpb(Node_map_std const& snodes)
   : snodes_(snodes) {}

   Node const& operator[](const Node_id id) const {
      return
               id < snodes_.node_id_next() ?
                        snodes_[id] :
                        static_cast<Super const&>(*this).node_map()[id];
   }

   /**
    @param id node ID
    @return immutable reference to node object with specified ID
    @throw Rdf_err if @b id does not exist
   */
   Node const& at(const Node_id id) const {
      return
               id < snodes_.node_id_next() ?
                        snodes_.at(id) :
                        static_cast<Super const&>(*this).node_map().at(id);
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
      return
               snodes_.is_constant(iid) ?
                        snodes_.find(iid) :
                        static_cast<Super const&>(*this).node_map().find(iid);
   }

   node_range find(Node const& node) const {
      return
               snodes_.is_constant(node.ns_id()) || node.empty() ?
                        snodes_.find(node) :
                        static_cast<Super const&>(*this).node_map().find(node);
   }

   /**@brief Insert IRI node
    @param nsid namespace IRI id
    @param name fragment name
    @return node ID
   */
   Node_id insert_iri(const Ns_id nsid, std::string const& name) {
      typedef typename Super::Err Err;
      if( is_empty(nsid)  ) {
         if( name.empty() ) return terms::T_empty_::id();
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("empty namespace for IRI node")
                  << typename Err::str1_t(name)
         );
      }
      if( is_blank(nsid) )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("blank namespace for IRI node")
                  << typename Err::str1_t("_" + name)
         );

      const Node node(nsid, name);
      if( snodes_.is_constant(nsid) ) {
         const node_range r = snodes_.find(node);
         if( r ) return r.front();
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("new term cannot be inserted into standard namespace")
                  << typename Err::str1_t( name )
                  << typename Err::str2_t( snodes_.at(nsid) )
         );
      }
      Super& self = static_cast<Super&>(*this);
      const node_range nr = self.node_map().find(node);
      if( nr ) return nr.front();
      return self.insert_new(nsid, name);
   }

protected:
   Node_id snode_id_next() const {return snodes_.node_id_next();}

private:
   Node_map_std const& snodes_;
};

}//namespace owlcpp
#endif /* NODES_IRI_STD_CRTPB_HPP_ */
