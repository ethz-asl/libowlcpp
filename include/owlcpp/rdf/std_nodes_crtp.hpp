/** @file "/owlcpp/include/owlcpp/rdf/std_nodes_crtp.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STD_NODES_CRTP_HPP_
#define STD_NODES_CRTP_HPP_
#include "owlcpp/rdf/node_map_std.hpp"
#include "owlcpp/rdf/std_nodes.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
template<class Super> class Std_nodes_crtp {
public:
   Std_nodes_crtp(Node_map_std const& snodes)
   : snodes_(snodes) {}

   Node const& operator[](const Node_id id) const {
      return
               id < snodes_.node_id_next() ?
                        snodes_[id] :
                        static_cast<Super const&>(*this).base_map()[id];
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
                        static_cast<Super const&>(*this).base_map()[id];
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
   Node_map_std::ns_range find(const Ns_id iid) const {
      return
               snodes_.is_constant(iid) ?
                        snodes_.find(iid) :
                        static_cast<Super const&>(*this).base_map().find(iid);
   }

   Node_map_std::node_range find(Node const& node) const {
      return
               snodes_.is_constant(node.ns_id()) || node == Node(terms::T_empty_()) ?
                        snodes_.find(node) :
                        static_cast<Super const&>(*this).base_map().find(node);
   }


private:
   Node_map_std const& snodes_;
};

}//namespace owlcpp
#endif /* STD_NODES_CRTP_HPP_ */
