/** @file "/owlcpp/include/owlcpp/rdf/crtpb_ns_node_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CRTPB_NS_NODE_IRI_HPP_
#define CRTPB_NS_NODE_IRI_HPP_
#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/ns_iri.hpp"
#include "owlcpp/rdf/store_concepts.hpp"

namespace owlcpp{

/**Enable interaction between a map of namespaces and a map of nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> struct Crtpb_ns_node_iri {

   /**@brief if not already present, store IRI reference node
    @param iri node IRI string;
    consistent uniform representation of non-ascii characters is assumed
    (e.g., UTF-8, or %HH)
    @return node ID
   */
   Node_id insert_node_iri(std::string const& iri) {
      BOOST_CONCEPT_ASSERT((Ns_store<Super>));
      BOOST_CONCEPT_ASSERT((Iri_node_store<Super>));
      Super& super = static_cast<Super&>(*this);
      std::size_t n;
      const Ns_id iid = super.insert(remove_fragment(iri, n));
      try{
         return super.insert_node_iri( iid, iri.substr(n) );
      } catch(base_exception const&) {
         typedef typename Super::Err Err;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("error inserting IRI")
                  << typename Err::str1_t( iri )
                  << typename Err::nested_t(boost::current_exception())
         );
      }
   }

   Node_id const* find_node_iri(std::string const& iri) const {
      BOOST_CONCEPT_ASSERT((Ns_store<Super>));
      BOOST_CONCEPT_ASSERT((Iri_node_store<Super>));
      std::size_t n;
      Super const& super = static_cast<Super const&>(*this);
      Ns_id const*const ns_id = super.find(remove_fragment(iri, n));
      if( ! ns_id ) return 0;
      return super.find_node_iri( *ns_id, iri.substr(n) );
   }

};


}//namespace owlcpp
#endif /* CRTPB_NS_NODE_IRI_HPP_ */
