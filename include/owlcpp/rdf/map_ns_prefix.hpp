/** @file "/owlcpp/include/owlcpp/rdf/map_ns_prefix.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef MAP_NS_PREFIX_HPP_
#define MAP_NS_PREFIX_HPP_
#include <string>
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/member.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Map_ns_prefix {

   struct Pref_wrap {
      Pref_wrap(std::string const& prefix, const Ns_id nsid)
      :pref(prefix), id(nsid) {}
      std::string pref;
      Ns_id id;
   };

   typedef boost::multi_index_container<
            Pref_wrap,
            boost::multi_index::indexed_by<
               boost::multi_index::ordered_unique<
                  boost::multi_index::tag<struct pref_tag>,
                  boost::multi_index::member<Pref_wrap, std::string, &Pref_wrap::pref>
               >,
               boost::multi_index::ordered_unique<
                  boost::multi_index::tag<struct id_tag>,
                  boost::multi_index::member<Pref_wrap, Ns_id, &Pref_wrap::id>
               >
            >
   > pref_map_t;

   typedef pref_map_t::index<pref_tag>::type pref_index_t;
   typedef pref_index_t::const_iterator pref_iterator_t;
   typedef pref_map_t::index<id_tag>::type id_index_t;
   typedef id_index_t::const_iterator id_iterator_t;

public:
   struct Err : public Rdf_err {};

   std::string prefix(const Ns_id id) const {
      id_index_t const& index = pref_.get<id_tag>();
      const id_iterator_t iter = index.find(id);
      if( iter == index.end() ) return "";
      return iter->pref;
   }

   Ns_id const* find(std::string const& pref) const {
      pref_index_t const& index = pref_.get<pref_tag>();
      const pref_iterator_t iter = index.find(pref);
      if( iter == index.end() ) return 0;
      return &iter->id;
   }

   /**@brief set or clear a prefix for namespace IRI
    @param id namespace IRI ID
    @param pref prefix string for namespace IRI;
    if \b pref is empty, the existing prefix is cleared from the IRI;
    if \b id is invalid, behavior is undefined
   */
   void set(const Ns_id id, std::string const& pref = "") {
      id_index_t& id_index = pref_.get<id_tag>();
      const id_iterator_t id_iter = id_index.find(id);

      if( pref.empty() ) {
         if( id_iter != id_index.end() ) id_index.erase(id_iter);
         return;
      }
      if( id_iter != id_index.end() ) {
         if( id_iter->pref != pref ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("different IRI prefix has been set")
                  << Err::str1_t(pref)
                  << Err::str2_t(id_iter->pref)
         );
         return;
      }

      pref_index_t const& pref_index = pref_.get<pref_tag>();
      const pref_iterator_t pref_iter = pref_index.find(pref);
      if( pref_iter != pref_index.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("prefix used for different IRI")
               << Err::str1_t(pref)
      );
      pref_.insert(Pref_wrap(pref, id));
   }

   void erase(const Ns_id id) {pref_.get<id_tag>().erase(id);}

   void clear() {pref_.clear();}

private:
   pref_map_t pref_;
};

}//namespace owlcpp
#endif /* MAP_NS_PREFIX_HPP_ */
