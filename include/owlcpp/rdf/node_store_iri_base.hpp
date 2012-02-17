/** @file "/owlcpp/include/owlcpp/rdf/node_store_iri_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_IRI_BASE_HPP_
#define NODE_STORE_IRI_BASE_HPP_
#include <string>
#include "boost/exception_ptr.hpp"
#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"

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
      const Ns_id iid =
               std::string::npos == n ?
                        self.iris().insert(iri) :
                        self.iris().insert(iri.substr(0,n));
      const std::string name = std::string::npos == n ? "" : iri.substr(n+1);
      try{
         return self.nodes().insert_iri( iid, name );
      } catch(...) {
         typedef typename T::Err Err;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("error inserting IRI")
                  << typename Err::str1_t( name )
                  << typename Err::str2_t( self.iris()[iid] )
                  << typename Err::nested_t(boost::current_exception())
         );
      }
   }


   /**
    @param iri node IRI
    @return pointer to node ID or NULL if not found.
   */
   Node_id const* find_iri_node(std::string const& iri) const {
      T const& self = static_cast<T const&>(*this);
      const std::size_t n = iri.find('#');
      Ns_id const * iid;
      typedef typename T::node_map_t::node_range node_range;
      node_range r;
      if( std::string::npos == n ) {
         iid = self.iris().find_iri(iri);
         if( ! iid ) return 0;
         r = self.nodes().find( Node(*iid, ""));
      } else {
         iid = self.iris().find_iri(iri.substr(0, n));
         if( ! iid ) return 0;
         r = self.nodes().find( Node(*iid, iri.substr(n+1)));
      }
      if( r ) return &r.front();
      return 0;
   }

   std::string string(const Node_id nid) const {
      T const& self = static_cast<T const&>(*this);
      Node const& node = self.nodes()[nid];
      const std::string name = node.value_str();
      if( name.empty() ) return self.iris()[node.ns_id()];
      return self.iris()[node.ns_id()] + '#' + name;
      //TODO: deal with blanks and literals
   }

};

}//namespace owlcpp
#endif /* NODE_STORE_IRI_BASE_HPP_ */
