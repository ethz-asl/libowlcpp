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
//#include "boost/iterator/transform_iterator.hpp"
#include "owlcpp/config.hpp"
#include "owlcpp/ns_id.hpp"
//#include "owlcpp/detail/object_store.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/detail/transform_iterator.hpp"

namespace owlcpp{

/**@brief Map for namespace IRIs
*******************************************************************************/
class Ns_map_base {
   typedef std::map<std::string,Ns_id> map_t;
   typedef map_t::const_iterator map_iter_t;
   typedef std::vector<std::pair<map_iter_t,map_iter_t> > vec_t;

   struct Make_id : public std::unary_function<map_iter_t, Ns_id> {
      Ns_id operator()(const map_iter_t i) {return i->second;}
   };

public:
   typedef Ns_id id_type;
   typedef Transform_iterator<Make_id, map_iter_t> const_iterator;
   typedef const_iterator iterator;

   struct Err : public Rdf_err {};

   std::size_t size() const {
      BOOST_ASSERT(id_.size() >= iri_.size());
      return iri_.size();
   }

   const_iterator begin() const {return iri_.begin();}
   const_iterator end() const {return iri_.end();}
   bool have(const Ns_id id) const {return ns_.have(id());}
   std::string operator[](const Ns_id id) const {return ns_[id()]->first;}
   std::string at(const Ns_id id) const {return ns_.at(id())->first;}

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   const_iterator find_iri(std::string const& iri) const {
      return const_iterator(ns_.find(iri));
   }

   Ns_id insert(std::string const& iri) {return Ns_id(ns_.insert(iri));}
   Ns_id insert(const Ns_id id, std::string const& iri) {
      return Ns_id(ns_.insert(id(), iri));
   }



private:
   map_t iri_;
   map_t pref_;
   vec_t id_;
};

}//namespace owlcpp
#endif /* NS_MAP_BASE_HPP_ */
