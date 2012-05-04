/** @file "/owlcpp/include/owlcpp/rdf/ns_map_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NS_MAP_BASE_HPP_
#define NS_MAP_BASE_HPP_
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "boost/assert.hpp"
#include "owlcpp/config.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/detail/transform_iterator.hpp"

namespace owlcpp{

/**@brief Map for namespace IRIs
*******************************************************************************/
class Ns_map_base {
   typedef std::map<std::string,Ns_id> map_t;
   typedef map_t::iterator map_iter_t;
   typedef std::pair<map_iter_t,map_iter_t> id_value_t;
   typedef std::vector<id_value_t> vec_t;
   typedef map_t::const_iterator map_citer_t;

   struct Make_id : public std::unary_function<map_citer_t, Ns_id> {
      Ns_id operator()(const map_citer_t i) {return i->second;}
   };

public:
   typedef Ns_id id_type;
   typedef Transform_iterator<Make_id, map_citer_t> const_iterator;
   typedef const_iterator iterator;

   struct Err : public Rdf_err {};

   std::size_t size() const {
      BOOST_ASSERT(id_.size() >= iri_.size());
      return iri_.size();
   }

   const_iterator begin() const {return iri_.begin();}
   const_iterator end() const {return iri_.end();}

   bool have(const Ns_id id) const {
      return id() < id_.size() && id_[id()].first != iri_.end();
   }

   std::string operator[](const Ns_id id) const {
      BOOST_ASSERT(have(id));
      return id_[id()].first->first;
   }

   std::string at(const Ns_id id) const {
      if( ! have(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid namespace ID")
               << Err::int1_t(id())
      );
      return id_[id()].first->first;
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find_iri(std::string const& iri) const {
      const map_citer_t i = iri_.find(iri);
      if( i == iri_.end() ) return 0;
      return &i->second;
   }

   std::string prefix(const Ns_id id) const {
      BOOST_ASSERT( have(id) );
      id_value_t const& v = id_[id()];
      if( v.second == pref_.end() ) return "";
      return v.second->first;
   }

   Ns_id const* find_prefix(std::string const& pref) const {
      const map_citer_t i = pref_.find(pref);
      if( i == pref_.end() ) return 0;
      return &i->second;
   }

   Ns_id insert(std::string const& iri) {
      const map_iter_t i = iri_.find(iri);
      if( i != iri_.end() ) return i->second;
      return insert(next_id(), iri);
   }

   Ns_id insert(const Ns_id id, std::string const& iri) {
      if( id() >= id_.size() ) {
         id_.resize(id() + 1, std::make_pair(iri_.end(), pref_.end()));
      }
      id_value_t& v = id_[id()];
      if( v.first != iri_.end() ) {
         if( v.first->first != iri ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("ID already in use")
                  << Err::str1_t(iri)
                  << Err::str2_t(v.first->first)
                  << Err::int1_t(id())
         );
         BOOST_ASSERT(iri_.find(iri) == v.first);
         return id;
      }
      BOOST_ASSERT(iri_.find(iri) == iri_.end());
      v.first = iri_.insert(std::make_pair(iri, id)).first;
      v.second = pref_.end();
      return id;
   }

   /**@brief set or clear a prefix for namespace IRI
    @param id namespace IRI ID
    @param pref prefix string for namespace IRI;
    if \b pref is empty, the existing prefix is cleared from the IRI
   */
   void set_prefix(const Ns_id id, std::string const& pref = "") {
      BOOST_ASSERT( have(id) );
      id_value_t& v = id_[id()];

      if( pref.empty() ) {
         if( v.second != pref_.end() ) {
            pref_.erase(v.second);
            v.second = pref_.end();
         }
         return;
      }

      if( v.second != pref_.end() ) {
         if( v.second->first != pref ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("different IRI prefix has been set")
                  << Err::str1_t(pref)
                  << Err::str2_t(v.second->first)
                  << Err::str3_t(at(id))
         );
         BOOST_ASSERT(pref_.find(pref) == v.second);
         return;
      }

      if( pref_.find(pref) != pref_.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("prefix used for different IRI")
               << Err::str1_t(pref)
               << Err::str2_t(v.second->first)
               << Err::str3_t(at(id))
      );

      v.second = pref_.insert(std::make_pair(pref, id)).first;
   }

   void remove(const Ns_id id) {
      if( ! have(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("removing non-existing IRI ID")
               << Err::int1_t(id())
      );
      id_value_t& v = id_[id()];
      iri_.erase(v.first);
      v.first = iri_.end();
      if( v.second != pref_.end() ) pref_.erase(v.second);
      erased_.push_back(id);
   }

   void clear() {
      iri_.clear();
      pref_.clear();
      id_.clear();
      erased_.clear();
   }

private:
   map_t iri_;
   map_t pref_;
   vec_t id_;
   std::vector<Ns_id> erased_;

   Ns_id next_id() {
      if( erased_.empty() ) return Ns_id(id_.size());
      const Ns_id id = erased_.back();
      erased_.pop_back();
      return id;
   }
};

}//namespace owlcpp
#endif /* NS_MAP_BASE_HPP_ */
