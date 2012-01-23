/** @file "/owlcpp/include/owlcpp/rdf/node_store_iri_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_IRI_BASE_HPP_
#define NODE_STORE_IRI_BASE_HPP_
#include "owlcpp/rdf/node_map.hpp"

namespace owlcpp{

/** CRTP base for RDF node stores
*******************************************************************************/
template<class T> struct Node_store_iri_base {

   /**@brief if not already present, store IRI reference node
    @param iri node IRI string;
    consistent uniform representation of non-ascii characters is assumed
    (e.g., UTF-8, or %HH)
    @return node ID
   */
   Node_id insert_iri_node(std::string const& iri) {
      T& self = static_cast<T&>(*this);
      const std::size_t n = iri.find('#');
      if( std::string::npos == n ) {
         const Ns_id nid = self.iris().insert(iri);
         return self.nodes().insert(Node(nid, ""));
      }
      const Ns_id nid = self.iris().insert(iri.substr(0,n));
      return self.nodes().insert( Node(nid, iri.substr(n+1)) );
   }

   /**
    @param iri node IRI
    @return pointer to node ID or NULL if not found.
   */
   Node_id const* find_iri_node(std::string const& iri) const {
      T const& self = static_cast<T const&>(*this);
      const std::size_t n = iri.find('#');
      Ns_id const * iid;
      if( std::string::npos == n ) {
         iid = self.iris().find_iri(iri);
         if( ! iid ) return 0;
         return self.nodes().find( Node(*iid, ""));
      } else {
         iid = self.iris().find_iri(iri.substr(0, n));
         if( ! iid ) return 0;
         return self.nodes().find( Node(*iid, iri.substr(n+1)));
      }
   }

};

}//namespace owlcpp
#endif /* NODE_STORE_IRI_BASE_HPP_ */
