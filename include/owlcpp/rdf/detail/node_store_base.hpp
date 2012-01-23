/** @file "/owlcpp/include/owlcpp/rdf/detail/node_store_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_BASE_HPP_
#define NODE_STORE_BASE_HPP_
#include <string>

#include "owlcpp/node_id.hpp"

namespace owlcpp{

/** CRTP base
*******************************************************************************/
template<class T> struct Node_store_base {

   /**@brief if not already present, store IRI reference node
    @param iri node IRI string;
    consistent uniform representation of non-ascii characters is assumed
    (e.g., UTF-8, or %HH)
    @return node ID
   */
   Node_id insert_iri_node(std::string const& iri) {
      T const& self = static_cast<T const&>(*this);
      const std::size_t n = iri.find('#');
      if( std::string::npos == n ) {
         const Ns_id nid = self.iris().insert(iri);
         return self.nodes().insert(Node(nid, ""));
      }
      const Ns_id nid = iri_.insert(iri.substr(0,n));
      return node_.insert( Node(nid, iri.substr(n+1)) );
   }

   /**
    @param iri node IRI
    @return pointer to node ID or NULL if not found.
   */
   Node_id const* find_iri_node(std::string const& iri) const;

};

}//namespace owlcpp
#endif /* NODE_STORE_BASE_HPP_ */
