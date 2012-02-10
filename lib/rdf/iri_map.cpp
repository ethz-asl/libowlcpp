/** @file "/owlcpp/lib/rdf/iri_map.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/iri_map.hpp"

#include "boost/mpl/for_each.hpp"

#include "iri_tag_vector.hpp"
#include "owlcpp/rdf/detail/iri_tag_inserter.hpp"

namespace owlcpp {

/*
*******************************************************************************/
Iri_map::Iri_map() {
   detail::Iri_tag_inserter iti(*this);
   boost::mpl::for_each<terms::mpl_vector_iris_t>(iti);
}

/*
*******************************************************************************/
void Iri_map::insert(const id_type id, std::string const& iri, std::string const& prefix) {
   BOOST_ASSERT(
            store_iri_.get<id_tag>().find(id) == store_iri_.get<id_tag>().end()
   );
   BOOST_ASSERT( ! find_iri(iri) );
   BOOST_ASSERT(
            store_pref_.get<id_tag>().find(id) == store_pref_.get<id_tag>().end()
   );
   BOOST_ASSERT( ! find_prefix(prefix) );

   store_iri_.insert(std::make_pair(id, iri));
   store_pref_.insert(std::make_pair(id, prefix));
   tracker_.ensure_min(id);
}

/*
*******************************************************************************/
void Iri_map::insert_prefix(const id_type id, std::string const& prefix) {
   BOOST_ASSERT(
            store_iri_.get<id_tag>().find(id) != store_iri_.get<id_tag>().end()
   );
   string_index_t const& pref_s_i = store_pref_.get<string_tag>();
   const string_iter_t pref_s_iter = pref_s_i.find(prefix);
   if( pref_s_iter == pref_s_i.end() ) {
      store_pref_.insert(std::make_pair(id, prefix));
   } else {
      const id_type id2 = pref_s_iter->first;
      if( id != id2 ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("inserting prefix used for another IRI")
               << Err::str1_t(prefix)
               << Err::str2_t((*this)[id])
               << Err::str3_t((*this)[id2])
      );
   }
}

/*
*******************************************************************************/
void Iri_map::remove(const id_type id) {
   id_index_t & id_index = store_iri_.get<id_tag>();
   id_iter_t i = id_index.find(id);
   if( i == id_index.end() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("removing non-existing IRI ID")
            << Err::int1_t(id())
   );
   id_index.erase(id);
   store_pref_.get<id_tag>().erase(id);
   tracker_.push(id);
}

/*
*******************************************************************************/
void Iri_map::remove(std::string const& iri) {
   string_index_t& string_index = store_iri_.get<string_tag>();
   string_iter_t i = string_index.find(iri);
   if( i == string_index.end() ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("removing non-existing IRI")
            << Err::str1_t(iri)
   );
   const id_type iid = i->first;
   string_index.erase(i);
   store_pref_.get<id_tag>().erase(iid);
   tracker_.push(iid);
}

}//namespace owlcpp
