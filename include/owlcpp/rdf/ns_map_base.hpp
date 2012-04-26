/** @file "/owlcpp/include/owlcpp/rdf/ns_map_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NS_MAP_BASE_HPP_
#define NS_MAP_BASE_HPP_
#include <string>
#include <functional>
#include "boost/assert.hpp"
#include "boost/iterator/transform_iterator.hpp"
#include "owlcpp/config.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/detail/object_store.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{

/**@brief Map for namespace IRIs
*******************************************************************************/
class Ns_map_base {
   typedef detail::Object_store<std::string> ns_store_t;

   struct Make_id : public std::unary_function<ns_store_t::const_iterator, Ns_id> {
      Ns_id operator()(const ns_store_t::const_iterator i) {return Ns_id(i->second);}
   };

public:
   typedef Ns_id id_type;
private:

public:
   typedef boost::transform_iterator<Make_id, ns_store_t::const_iterator> const_iterator;
   typedef const_iterator iterator;

   struct Err : public Rdf_err {};

   std::size_t size() const {return ns_.size();}
   const_iterator begin() const {return iterator(ns_.begin());}
   const_iterator end() const {return iterator(ns_.end());}

private:
   ns_store_t ns_;
};

}//namespace owlcpp
#endif /* NS_MAP_BASE_HPP_ */
