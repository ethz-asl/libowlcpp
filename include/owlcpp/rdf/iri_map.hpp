/** @file "/owlcpp/include/owlcpp/rdf/iri_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef IRI_MAP_HPP_
#define IRI_MAP_HPP_
#include <string>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{ namespace detail{

class Iri_tag_inserter;

}//namespace detail

/**@brief Store namespace IRIs
*******************************************************************************/
class OWLCPP_RDF_DECL Iri_map {
public:
   typedef Ns_id id_type;
   typedef std::pair<id_type, std::string> value_t;
private:
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

   void insert(const id_type iid, std::string const& iri, std::string const& prefix);

   friend class detail::Iri_tag_inserter;

public:
   typedef store_t::iterator iterator;
   typedef iterator const_iterator;

   struct Err : public base_exception {};
   Iri_map();

   std::size_t size() const {return store_iri_.size();}

   std::string operator[](const id_type iid) const {
      BOOST_ASSERT(store_iri_.get<id_tag>().find(iid) != store_iri_.get<id_tag>().end());
      return store_iri_.get<id_tag>().find(iid)->second;
   }

   std::string at(const id_type iid) const {
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
    @return IRI prefix string or "" if no prefix was defined
   */
   std::string find_prefix(const id_type iid) const {
      id_index_t const& id_index = store_pref_.get<id_tag>();
      id_iter_t id_iter = id_index.find(iid);
      if( id_iter == id_index.end() ) return "";
      return id_iter->second;
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

   void remove(const id_type id);
   void remove(std::string const& iri);
   const_iterator begin() const {return store_iri_.begin();}
   const_iterator end() const {return store_iri_.end();}

protected:
   detail::Id_tracker<id_type> tracker_;
   store_t store_iri_;
   store_t store_pref_;
};


/** Copy IRIs and prefixes from one IRI map to another and insert pairs of
 old and new IRI IDs into @b id_map.
 @param im IRI map to copy from
 @param id_map an IRI ID map to insert IRI ID pairs
*******************************************************************************/
template<class IriMap1, class IriMap2, class IriIdMap> inline void
copy_iris(IriMap1 const& im1, IriMap2& im2, IriIdMap& id_map) {
   typedef typename IriMap1::value_t pair_t;
   typedef typename IriMap2::id_type id_t;
   BOOST_FOREACH(pair_t const& p, im1) {
      const id_t id = im2.insert(p.second);
      const std::string pref = im1.find_prefix(p.first);
      if( ! pref.empty() && im2.find_prefix(id).empty() ) im2.insert_prefix(id, pref);
      id_map.insert(std::make_pair(p.first, id));
   }
}

}//namespace owlcpp
#endif /* IRI_MAP_HPP_ */
