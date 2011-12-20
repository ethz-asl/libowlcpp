/** @file "/owl_cpp/include/owl_cpp/catalog.hpp"
part of %owl_cpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#ifndef CATALOG_HPP_
#define CATALOG_HPP_
#include <string>
#include "boost/range.hpp"
namespace b = boost;
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/identity.hpp"
#include "boost/multi_index/member.hpp"
namespace bm = boost::multi_index;

#include "owl_cpp/io/config.hpp"
#include "owl_cpp/exception.hpp"

namespace owl_cpp{

/**@brief Store locations, ontology IRIs, and version IRIs of OWL ontologies.
@details Locations should unique; ontology IRIs may be repeated;
non-empty verions IRIs should be unique.
Ontology ID is pair of ontology IRI and version IRI.
Ontology ID should be unique.
*******************************************************************************/
class OWLCPP_IO_DECL Catalog {
   struct Location {
      Location(const std::string& path_, const std::string& iri_, const std::string& version_)
      : path(path_), iri(iri_), version(version_) {}

      std::string path;
      std::string iri;
      std::string version;
   };

   typedef bm::multi_index_container<
         Location,
         bm::indexed_by<
            bm::ordered_unique<     bm::member<Location, std::string, &Location::path> >,
            bm::ordered_non_unique< bm::member<Location, std::string, &Location::iri> >,
            bm::ordered_non_unique< bm::member<Location, std::string, &Location::version> >
      >
   > stor_t;

   typedef stor_t::nth_index<0>::type by_path_t;
   typedef by_path_t::iterator path_iter_t;
   typedef stor_t::nth_index<1>::type by_iri_t;
   typedef by_iri_t::iterator iri_iter_t;
   typedef b::iterator_range<iri_iter_t> iri_range_t;
   typedef stor_t::nth_index<2>::type by_version_t;
   typedef by_version_t::iterator version_iter_t;

public:
   typedef b::iterator_range<path_iter_t> location_range_t;
   struct Err : public base_exception {};
   struct Not_found_err : public base_exception {};

   /**@brief Insert new ontology location.
   @details Repeated insertion of same @a path, @a iri, or @a version has no effect.
   @throws Err if @a path is empty
   @throws Err if @a iri is empty
   @throws Err if duplicate @a path, different @a iri / @a version
   @throws Err if different @a path, @a duplicate @a iri and @a version
   */
   void insert(
         const std::string& path,/**< path to ontology file */
         const std::string& iri, /**< ontology IRI */
         const std::string& version /**< ontology version IRI */
   );

   /**@brief Find ontology location
    @throws Not_found_err    */
   const std::string& find_location(
         const std::string& iri /**<ontology IRI or version*/
   ) const;

   location_range_t locations() const {return b::make_iterator_range(stor_.get<0>());}

private:
   stor_t stor_;
};

}//namespace owl_cpp

#endif /* CATALOG_HPP_ */
