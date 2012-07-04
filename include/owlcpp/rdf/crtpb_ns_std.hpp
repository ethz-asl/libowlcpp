/** @file "/owlcpp/include/owlcpp/rdf/crtpb_ns_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CRTPB_NS_STD_HPP_
#define CRTPB_NS_STD_HPP_
#include <string>
#include "boost/assert.hpp"

#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/ns_id.hpp"

namespace owlcpp{

/** Enable interaction between a map of user namespaces and
a map of standard namespaces.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Crtpb_ns_std {

   typedef typename Map_traits<Super>::map_ns_t map_ns_t;
   typedef typename Map_traits<Super>::map_node_std_t map_node_std_t;

   map_ns_t const& ns() const {
      return static_cast<Super const&>(*this).namespaces();
   }

   map_ns_t& ns() {
      return static_cast<Super&>(*this).ns_;
   }

   map_node_std_t const& smap() const {
      return static_cast<Super const&>(*this).snode_;
   }

public:
   typedef Super self_type;

   std::string operator[](const Ns_id id) const {
      return id < smap().ns_id_next() ? smap()[id] : ns()[id];
   }

   std::string at(const Ns_id id) const {
      return id < smap().ns_id_next() ? smap().at(id) : ns().at(id);
   }

   bool valid(const Ns_id nsid) const {
      BOOST_ASSERT(! (smap().have(nsid) && ns().have(nsid)) );
      return smap().have(nsid) || ns().have(nsid);
   }

   /**
    @param iid namespace IRI ID
    @return IRI prefix string or "" if no prefix was defined
   */
   std::string prefix(const Ns_id iid) const {
      if( iid < smap().ns_id_next() ) return smap().prefix(iid);
      return ns().prefix(iid);
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find_ns(std::string const& iri) const {
      if( Ns_id const*const id = smap().find_iri(iri) ) return id;
      return ns().find_iri(iri);
   }

   /**
    @param pref prefix for namespace IRI
    @return pointer to namespace IRI ID or NULL if prefix is unknown
   */
   Ns_id const* find_prefix(std::string const& pref) const {
      if( Ns_id const*const id = smap().find_prefix(pref) ) return id;
      return ns().find_prefix(pref);
   }

   Ns_id insert_ns(std::string const& iri) {
      if( Ns_id const*const iid = find_ns(iri) ) return *iid;
      return ns().insert(iri);
   }

   /**
    @param iid namespace IRI ID
    @param pref namespace IRI prefix
    @details Define or re-define the prefix for a user-defined namespace IRI.
   */
   void insert_prefix(const Ns_id iid, std::string const& pref) {
      typedef typename Super::Err Err;
      if( iid < smap().ns_id_next() ) {
         if( pref.empty() || pref == smap().prefix(iid) ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("cannot re-define standard prefix")
                  << typename Err::str1_t(pref)
                  << typename Err::str2_t(smap().prefix(iid))
         );
      }
      BOOST_ASSERT( ns().have(iid) );
      if( pref.empty() ) {
         ns().set_prefix(iid);
         return;
      }
      Ns_id const*const iid0 = ns().find_prefix(pref);
      if( iid0 ) {
         if( *iid0 == iid ) return; //prefix already defined for same IRI
         BOOST_THROW_EXCEPTION(
                  Err()
                  << typename Err::msg_t("prefix reserved for different IRI")
                  << typename Err::str1_t(pref)
                  << typename Err::str2_t(at(iid))
                  << typename Err::str3_t(at(*iid0))
         );
      }
      ns().set_prefix(iid, pref);
   }

};

}//namespace owlcpp
#endif /* CRTPB_NS_STD_HPP_ */
