/** @file "/owlcpp/include/owlcpp/rdf/iri_store.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011-2
*******************************************************************************/
#ifndef IRI_STORE_HPP_
#define IRI_STORE_HPP_
#include <string>
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{
class Iri_tag_inserter;
}

/**@brief Store namespace IRIs
*******************************************************************************/
class OWLCPP_RDF_DECL Iri_store {
public:
   typedef Ns_id id_type;
private:
   typedef std::pair<id_type, std::string> entry_t;

   typedef boost::multi_index_container<
         entry_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  entry_t, id_type, &entry_t::first
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct string_tag>,
               boost::multi_index::member<
                  entry_t, std::string, &entry_t::second
               >
            >
         >
      > store_t;
   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef store_t::index<string_tag>::type string_index_t;
   typedef string_index_t::iterator string_iter_t;

   void insert(const id_type iid, std::string const& iri, std::string const& prefix);

   friend class detail::Iri_tag_inserter;

public:
   typedef store_t::iterator iterator;

   struct Err : public base_exception {};
   Iri_store();

   std::size_t size() const {return store_iri_.size();}

   std::string const& operator[](const id_type iid) const {
      BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) != store_iri_.get<id_tag>().end());
      return store_iri_.get<id_tag>().find(iid)->second;
   }

   std::string const& at(const id_type iid) const {
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
   id_type const* find_iri(std::string const& iri) const {
      string_index_t const& s_index = store_iri_.get<string_tag>();
      const string_iter_t s_iter = s_index.find(iri);
      if( s_iter == s_index.end() ) return 0;
      return &s_iter->first;
   }

   /**
    @param iid namespace IRI ID
    @return pointer to prefix string or NULL if no prefix was defined
   */
   std::string const* find_prefix(const id_type iid) const {
      id_index_t const& id_index = store_pref_.get<id_tag>();
      id_iter_t id_iter = id_index.find(iid);
      if( id_iter == id_index.end() ) return 0;
      return &id_iter->second;
   }

   /**
    @param pref prefix for namespace IRI
    @return pointer to namespace IRI ID or NULL if prefix is unknown
   */
   id_type const* find_prefix(std::string const& pref) const {
      string_index_t const& s_index = store_pref_.get<string_tag>();
      const string_iter_t s_iter = s_index.find(pref);
      if( s_iter == s_index.end() ) return 0;
      BOOST_ASSERT(
               store_iri_.get<id_tag>().find(s_iter->first) !=
                        store_iri_.get<id_tag>().end()
      );
      return &s_iter->first;
   }

   void insert_prefix(const id_type iid, std::string const& prefix);

   id_type insert(std::string const& iri) {
      string_index_t const& iri_i = store_iri_.get<string_tag>();
      const string_iter_t iri_s_iter = iri_i.find(iri);
      if( iri_s_iter == iri_i.end() ) {
         const id_type iid = tracker_.get();
         BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) == store_iri_.get<id_tag>().end());
         BOOST_ASSERT(store_pref_.get<id_tag>().find(iid) == store_pref_.get<id_tag>().end());
         store_iri_.insert(std::make_pair(iid, iri));
         return iid;
      }
      return iri_s_iter->first;
   }

   id_type insert(std::string const& iri, std::string const& prefix) {
      string_index_t const& iri_i = store_iri_.get<string_tag>();
      const string_iter_t iri_s_iter = iri_i.find(iri);
      string_index_t const& pref_i = store_pref_.get<string_tag>();
      const string_iter_t pref_s_iter = pref_i.find(prefix);
      if( iri_s_iter == iri_i.end() ) {
         const id_type iid = tracker_.get();
         BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) == store_iri_.get<id_tag>().end());
         BOOST_ASSERT(store_pref_.get<id_tag>().find(iid) == store_pref_.get<id_tag>().end());

         if( pref_s_iter != pref_i.end() ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("same prefix for two IRIs")
                  << Err::str1_t(prefix)
                  << Err::str2_t(iri)
                  << Err::str3_t((*this)[pref_s_iter->first])
         );
         store_iri_.insert(std::make_pair(iid, iri));
         store_pref_.insert(std::make_pair(iid, prefix));
         return iid;
      }
      const id_type iid = iri_s_iter->first;
      if( pref_s_iter == pref_i.end() ) {
         store_pref_.insert(std::make_pair(iid, prefix));
      } else {
         const id_type id2 = pref_s_iter->first;
         if( iid != id2 ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("inserting prefix used for another IRI")
                  << Err::str1_t(prefix)
                  << Err::str2_t(iri)
                  << Err::str3_t((*this)[id2])
         );
      }
      return iid;
   }

   void remove(const id_type id);
   void remove(std::string const& iri);
   iterator begin() const {return store_iri_.begin();}
   iterator end() const {return store_iri_.end();}

protected:
   detail::Id_tracker<id_type> tracker_;
   store_t store_iri_;
   store_t store_pref_;
};

}//namespace owlcpp
#endif /* IRI_STORE_HPP_ */
