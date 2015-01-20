/** @file "/owlcpp/include/owlcpp/rdf/map_std_ns_crtpb.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_STD_NS_CRTPB_HPP_
#define MAP_STD_NS_CRTPB_HPP_
#include "boost/assert.hpp"

#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/rdf/store_concepts.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**Enable interaction between a mutable maps of namespaces and nodes and
immutable map of standard IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_std_ns_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_std_type map_std_type;
   typedef typename traits::map_ns_type map_ns_type;

   map_std_type const& _map_std() const {
      return static_cast<Super const&>(*this).map_std_;
   }

   map_ns_type const& _map_ns() const {
      return static_cast<Super const&>(*this).map_ns_;
   }

   map_ns_type& _map_ns() {
      return static_cast<Super&>(*this).map_ns_;
   }

public:

   Ns_iri const& operator[](const Ns_id nsid) const {
      return _map_std().is_standard(nsid) ?
               _map_std()[nsid] : _map_ns()[nsid];
   }

   Ns_iri const& at(const Ns_id nsid) const {
      return _map_std().is_standard(nsid) ?
               _map_std().at(nsid) : _map_ns().at(nsid);
   }

   Ns_iri const* find(const Ns_id nsid) const {
      return _map_std().is_standard(nsid) ?
               _map_std().find(nsid) : _map_ns().find(nsid);
   }

   /**
    @param nsid namespace IRI ID
    @return IRI prefix string or "" if no prefix was defined
   */
   std::string prefix(const Ns_id nsid) const {
      return _map_std().is_standard(nsid) ?
               _map_std().prefix(nsid) : _map_ns().prefix(nsid);
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find(Ns_iri const& iri) const {
      if( Ns_id const*const id = _map_std().find(iri) ) return id;
      return _map_ns().find(iri);
   }

   /**
    @param pref prefix for namespace IRI
    @return pointer to namespace IRI ID or NULL if prefix is unknown
   */
   Ns_id const* find_prefix(std::string const& pref) const {
      if( Ns_id const*const id = _map_std().find_prefix(pref) ) return id;
      return _map_ns().find_prefix(pref);
   }

   Ns_id insert(Ns_iri const& iri) {
      if( Ns_id const*const iid = find(iri) ) return *iid;
      return _map_ns().insert(iri);
   }

   /**
    @param nsid namespace IRI ID
    @param pref namespace IRI prefix
    @details Define or re-define prefix for a user-defined namespace IRI.
   */
   void insert_prefix(const Ns_id nsid, std::string const& pref) {
      typedef typename Super::Err Err;
      if( nsid < detail::min_ns_id() ) {
         if( pref.empty() || pref == _map_std().prefix(nsid) ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("cannot re-define standard prefix")
                  << typename Err::str1_t(pref)
                  << typename Err::str2_t(_map_std().prefix(nsid))
         );
      }
      BOOST_ASSERT( _map_ns().find(nsid) );
      if( pref.empty() ) {
         _map_ns().set_prefix(nsid);
         return;
      }
      Ns_id const*const iid0 = _map_ns().find_prefix(pref);
      if( iid0 ) {
         if( *iid0 == nsid ) return; //prefix already defined for same IRI
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("prefix reserved for different IRI")
                  << typename Err::str1_t(pref)
                  << typename Err::str2_t(at(nsid).str())
                  << typename Err::str3_t(at(*iid0).str())
         );
      }
      _map_ns().set_prefix(nsid, pref);
   }
};

}//namespace owlcpp
#endif /* MAP_STD_NS_CRTPB_HPP_ */
