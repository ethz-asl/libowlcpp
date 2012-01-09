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
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/tuple/tuple.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{
class Iri_tag_inserter;
}

/**@brief Store namespace IRIs
*******************************************************************************/
class Iri_store {
public:
   typedef Ns_id id_type;
private:
   typedef boost::tuple<id_type, std::string> entry_t;

   typedef boost::multi_index_container<
         entry_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::const_mem_fun<
                  entry_t::inherited, id_type const&, &entry_t::get<0>
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct string_tag>,
               boost::multi_index::const_mem_fun<
                  entry_t::inherited, std::string const&, &entry_t::get<1>
               >
            >
         >
      > store_t;
   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;
   typedef store_t::index<string_tag>::type string_index_t;
   typedef string_index_t::iterator string_iter_t;

   void insert(const id_type iid, std::string const& iri, std::string const& prefix) {
      BOOST_ASSERT(
               store_iri_.get<id_tag>().find(iid) == store_iri_.get<id_tag>().end()
      );
      BOOST_ASSERT( ! find_iri(iri) );
      BOOST_ASSERT(
               store_pref_.get<id_tag>().find(iid) == store_pref_.get<id_tag>().end()
      );
      BOOST_ASSERT( ! find_prefix(prefix) );

      store_iri_.insert(boost::make_tuple(iid, iri));
      store_pref_.insert(boost::make_tuple(iid, prefix));
   }

   friend class detail::Iri_tag_inserter;

protected:
   Iri_store(const std::size_t n0) : tracker_(n0) {}

public:
   struct Err : public base_exception {};
   Iri_store() : tracker_(0) {}

   std::size_t size() const {return store_iri_.size();}

   std::string const& operator[](const id_type iid) const {
      BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) != store_iri_.get<id_tag>().end());
      return store_iri_.get<id_tag>().find(iid)->get<1>();
   }

   std::string const& at(const id_type iid) const {
      id_index_t const& index = store_iri_.get<id_tag>();
      const id_iter_t iter = index.find(iid);
      if(iter == index.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown IRI ID")
               << Err::int1_t(iid())
      );
      return iter->get<1>();
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   id_type const* find_iri(std::string const& iri) const {
      string_index_t const& s_index = store_iri_.get<string_tag>();
      const string_iter_t s_iter = s_index.find(iri);
      if( s_iter == s_index.end() ) return 0;
      return &s_iter->get<0>();
   }

   /**
    @param iid namespace IRI ID
    @return pointer to prefix string or NULL if no prefix was defined
   */
   std::string const* find_prefix(const id_type iid) const {
      id_index_t const& id_index = store_pref_.get<id_tag>();
      id_iter_t id_iter = id_index.find(iid);
      if( id_iter == id_index.end() ) return 0;
      return &id_iter->get<1>();
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
               store_iri_.get<id_tag>().find(s_iter->get<0>()) !=
                        store_iri_.get<id_tag>().end()
      );
      return &s_iter->get<0>();
   }

   void insert_prefix(const id_type iid, std::string const& prefix) {
      id_index_t const& iri_i_i = store_iri_.get<id_tag>();
      const id_iter_t iri_i_iter = iri_i_i.find(iid);
      if( iri_i_iter == iri_i_i.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("inserting prefix for unknown IRI ID")
               << Err::str1_t(prefix)
               << Err::int1_t(iid())
      );
      string_index_t const& pref_s_i = store_pref_.get<string_tag>();
      const string_iter_t pref_s_iter = pref_s_i.find(prefix);
      if( pref_s_iter == pref_s_i.end() ) {
         store_pref_.insert(boost::make_tuple(iid, prefix));
      } else {
         const id_type id2 = pref_s_iter->get<0>();
         if( iid != id2 ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("inserting prefix used for another IRI")
                  << Err::str1_t(prefix)
                  << Err::str2_t((*this)[iid])
                  << Err::str3_t((*this)[id2])
         );
      }
   }

   id_type insert(std::string const& iri) {
      string_index_t const& iri_i = store_iri_.get<string_tag>();
      const string_iter_t iri_s_iter = iri_i.find(iri);
      if( iri_s_iter == iri_i.end() ) {
         const id_type iid = tracker_.get();
         BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) == store_iri_.get<id_tag>().end());
         BOOST_ASSERT(store_pref_.get<id_tag>().find(iid) == store_pref_.get<id_tag>().end());
         store_iri_.insert(boost::make_tuple(iid, iri));
         return iid;
      }
      return iri_s_iter->get<0>();
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
                  << Err::str3_t((*this)[pref_s_iter->get<0>()])
         );
         store_iri_.insert(boost::make_tuple(iid, iri));
         store_pref_.insert(boost::make_tuple(iid, prefix));
         return iid;
      }
      const id_type iid = iri_s_iter->get<0>();
      if( pref_s_iter == pref_i.end() ) {
         store_pref_.insert(boost::make_tuple(iid, prefix));
      } else {
         const id_type id2 = pref_s_iter->get<0>();
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

   void remove(const id_type iid) {
      id_index_t & id_index = store_iri_.get<id_tag>();
      id_iter_t i = id_index.find(iid);
      if( i == id_index.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("removing non-existing IRI ID")
               << Err::int1_t(iid())
      );
      id_index.erase(iid);
      store_pref_.get<id_tag>().erase(iid);
      tracker_.push(iid);
   }

   void remove(std::string const& iri) {
      string_index_t& string_index = store_iri_.get<string_tag>();
      string_iter_t i = string_index.find(iri);
      if( i == string_index.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("removing non-existing IRI")
               << Err::str1_t(iri)
      );
      const id_type iid = i->get<0>();
      string_index.erase(i);
      store_pref_.get<id_tag>().erase(iid);
      tracker_.push(iid);
   }

private:
   detail::Id_tracker<id_type> tracker_;
   store_t store_iri_;
   store_t store_pref_;
};

}//namespace owlcpp
#endif /* IRI_STORE_HPP_ */
