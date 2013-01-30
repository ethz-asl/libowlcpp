/** @file "/owlcpp/include/owlcpp/rdf/map_ns_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NS_CRTPB_HPP_
#define MAP_NS_CRTPB_HPP_

#include "owlcpp/detail/map_traits.hpp"

namespace owlcpp{

/**Expose namespace map interface.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_ns_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_ns_type map_ns_type;

   map_ns_type const& _map_ns() const {
      return static_cast<Super const&>(*this).map_ns_;
   }

   map_ns_type& _map_ns() {
      return static_cast<Super&>(*this).map_ns_;
   }

public:
   std::string operator[](const Ns_id nsid) const {
      return _map_ns()[nsid];
   }

   std::string at(const Ns_id nsid) const {
      return _map_ns().at(nsid);
   }

   /**
    @param nsid namespace IRI ID
    @return IRI prefix string or "" if no prefix was defined
   */
   std::string prefix(const Ns_id nsid) const {
      return _map_ns().prefix(nsid);
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find_ns(std::string const& iri) const {
      return _map_ns().find_iri(iri);
   }

   /**
    @param pref prefix for namespace IRI
    @return pointer to namespace IRI ID or NULL if prefix is unknown
   */
   Ns_id const* find_prefix(std::string const& pref) const {
      return _map_ns().find_prefix(pref);
   }

   Ns_id insert_ns(std::string const& iri) {
      return _map_ns().insert(iri);
   }

   /**
    @param nsid namespace IRI ID
    @param pref namespace IRI prefix
    @details Define or re-define the prefix for a user-defined namespace IRI.
   */
   void insert_prefix(const Ns_id nsid, std::string const& pref) {
      _map_ns().set_prefix(nsid, pref);
   }
};
}//namespace owlcpp
#endif /* MAP_NS_CRTPB_HPP_ */
