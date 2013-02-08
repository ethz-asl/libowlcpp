/** @file "/owlcpp/include/owlcpp/rdf/map_ns.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NS_HPP_
#define MAP_NS_HPP_
#include <map>
#include <vector>
#include <functional>
#include "boost/assert.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/ns_iri.hpp"
#include "owlcpp/rdf/detail/map_id_object.hpp"
#include "owlcpp/rdf/map_ns_prefix.hpp"
#include "owlcpp/detail/iterator_member_pair.hpp"
#include "owlcpp/detail/member_iterator.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**@brief Map for namespace IRIs
*******************************************************************************/
class Map_ns {
public:
   typedef Ns_id id_type;
private:

   typedef detail::Map_id_object<Ns_iri, id_type> map_t;

public:
   typedef Iterator_member_pair<map_t::const_iterator, id_type const, 2> iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   Map_ns(const Ns_id id0 = detail::min_ns_id())
   : iri_(id0), pref_()
   {}

   std::size_t size() const {return iri_.size();}
   const_iterator begin() const {return iri_.begin();}
   const_iterator end() const {return iri_.end();}
   bool empty() const {return iri_.empty();}
   Ns_iri const& operator[](const Ns_id id) const {return iri_[id];}
   Ns_iri const& at(const Ns_id id) const {return iri_.at(id);}
   Ns_iri const* find(const Ns_id id) const {return iri_.find(id);}
   std::string prefix(const Ns_id id) const {return pref_.prefix(id);}
   Ns_id const* find_prefix(std::string const& pref) const {return pref_.find(pref);}

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find(Ns_iri const& iri) const {return iri_.find(iri);}
   Ns_id const* find(std::string const& iri) const {return iri_.find(Ns_iri(iri));}

   Ns_id insert(Ns_iri const& iri) {return iri_.insert(iri);}
   Ns_id insert(std::string const& iri) {return insert(Ns_iri(iri));}

   void set_prefix(const Ns_id id, std::string const& pref = "") {
      if( ! find(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid namespace ID")
               << Err::int1_t(id())
      );
      pref_.set(id, pref);
   }

   void remove(const Ns_id id) {
      iri_.remove(id);
      pref_.erase(id);
   }

   void clear() {
      pref_.clear();
      iri_.clear();
   }

private:
   map_t iri_;
   Map_ns_prefix pref_;
};

}//namespace owlcpp
#endif /* MAP_NS_HPP_ */
