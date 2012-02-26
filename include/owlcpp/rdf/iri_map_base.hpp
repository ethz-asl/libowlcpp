/** @file "/owlcpp/include/owlcpp/rdf/iri_map_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef IRI_MAP_BASE_HPP_
#define IRI_MAP_BASE_HPP_
#include <string>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Iri_map_base {
public:
   typedef Ns_id id_type;
private:
   typedef std::pair<id_type, std::string> value_t;
   typedef boost::multi_index_container<
         value_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  value_t, id_type, &value_t::first
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct string_tag>,
               boost::multi_index::member<
                  value_t, std::string, &value_t::second
               >
            >
         >
      > store_t;
   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef store_t::index<string_tag>::type string_index_t;
   typedef string_index_t::iterator string_iter_t;

public:
   typedef Member_iterator<store_t::const_iterator, const Ns_id, &value_t::first> const_iterator;
   typedef const_iterator iterator;

   struct Err : public Rdf_err {};

   std::size_t size() const {return store_iri_.size();}
   const_iterator begin() const {return store_iri_.begin();}
   const_iterator end() const {return store_iri_.end();}

   bool have(const Ns_id iid) const {
      return store_iri_.get<id_tag>().find(iid) != store_iri_.get<id_tag>().end();
   }

   std::string operator[](const Ns_id iid) const {
      BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) != store_iri_.get<id_tag>().end());
      return store_iri_.get<id_tag>().find(iid)->second;
   }

   std::string at(const Ns_id iid) const {
      id_index_t const& index = store_iri_.get<id_tag>();
      const id_iter_t iter = index.find(iid);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown IRI ID")
               << Err::int1_t(iid())
      );
      return iter->second;
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find_iri(std::string const& iri) const {
      string_index_t const& s_index = store_iri_.get<string_tag>();
      const string_iter_t s_iter = s_index.find(iri);
      if( s_iter == s_index.end() ) return 0;
      return &s_iter->first;
   }

   /**
    @param iid namespace IRI ID
    @return IRI prefix string or "" if no prefix was defined
   */
   std::string prefix(const Ns_id iid) const {
      id_index_t const& id_index = store_pref_.get<id_tag>();
      id_iter_t id_iter = id_index.find(iid);
      if( id_iter == id_index.end() ) return "";
      return id_iter->second;
   }

   /**
    @param pref prefix for namespace IRI
    @return pointer to namespace IRI ID or NULL if prefix is unknown
   */
   Ns_id const* find_prefix(std::string const& pref) const {
      string_index_t const& s_index = store_pref_.get<string_tag>();
      const string_iter_t s_iter = s_index.find(pref);
      if( s_iter == s_index.end() ) return 0;
      BOOST_ASSERT(
               store_iri_.get<id_tag>().find(s_iter->first) !=
                        store_iri_.get<id_tag>().end()
      );
      return &s_iter->first;
   }

   /**
    @param iid namespace IRI ID; the IRI is not necessarily defined in this map
    @param pref namespace IRI prefix;
    @throw Err if prefix is already defined for for a different namespace IRI
   */
   void insert_prefix(const Ns_id iid, std::string const& pref) {
      Ns_id const*const iid0 = find_prefix(pref);
      if( iid0 ) {
         if( iid == *iid0 ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("prefix reserved for another IRI")
                  << Err::str1_t(pref)
                  << Err::str2_t(at(iid))
                  << Err::str3_t(at(*iid0))
         );
      }
      store_pref_.insert(std::make_pair(iid, pref));
   }

   Ns_id insert(const Ns_id iid, std::string const& iri) {
      BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) == store_iri_.get<id_tag>().end());
      BOOST_ASSERT(store_pref_.get<id_tag>().find(iid) == store_pref_.get<id_tag>().end());
      store_iri_.insert(std::make_pair(iid, iri));
      return iid;
   }

   template<class Tag> void insert_tag(Tag const&) {
      store_iri_.insert( std::make_pair(Tag::id(), Tag::iri()) );
      insert_prefix( Tag::id(), Tag::prefix() );
   }

   void remove(const Ns_id id) {
      id_index_t & id_index = store_iri_.get<id_tag>();
      id_iter_t i = id_index.find(id);
      if( i == id_index.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("removing non-existing IRI ID")
               << Err::int1_t(id())
      );
      id_index.erase(id);
      store_pref_.get<id_tag>().erase(id);
   }

   void clear() {
      store_pref_.clear();
      store_iri_.clear();
   }

private:
   store_t store_iri_;
   store_t store_pref_;
};

}//namespace owlcpp
#endif /* IRI_MAP_BASE_HPP_ */
