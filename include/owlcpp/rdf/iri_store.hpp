/** @file "/owlcpp/include/owlcpp/rdf/iri_store.hpp"
part of %owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#ifndef IRI_STORE_HPP_
#define IRI_STORE_HPP_
#include <string>
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/global_fun.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/tuple/tuple.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{

/**@brief Store namespace IRIs
*******************************************************************************/
class Iri_store {
public:
   typedef Ns_id id_type;
private:
   typedef boost::tuple<id_type,std::string> entry_t;

   namespace bmi = boost::multi_index;

   typedef boost::multi_index_container<
         entry_t,
         bmi::indexed_by<
            bmi::hashed_unique<
               bmi::tag<struct id_tag>,
               bmi::const_mem_fun<entry_t, id_type, &entry_t::get<0> >
            >,
            bmi::hashed_unique<
               bmi::tag<struct iri_tag>,
               bmi::const_mem_fun<entry_t, std::string, &entry_t::get<1> >
            >
         >
      > store_t;
public:
   Iri_store(std::size_t const n0) : tracker_(n0) {}

   id_type insert(std::string const& iri, std::string const& prefix = "") {

   }

private:
   detail::Id_tracker<id_type> tracker_;
   store_t store_iri_;
   store_t store_pref_;
};

}//namespace owlcpp
#endif /* IRI_STORE_HPP_ */
