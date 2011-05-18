/** @file "/owl_cpp/lib/catalog.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#include "owl_cpp/catalog.hpp"
#include "boost/foreach.hpp"

namespace owl_cpp {

/*
*******************************************************************************/
void Catalog::insert(
      const std::string& path,
      const std::string& iri,
      const std::string& version
) {
   if( path.empty() ) BOOST_THROW_EXCEPTION(
         Err() << Err::msg_t("empty ontology location")
   );

   if( iri.empty() ) BOOST_THROW_EXCEPTION(
         Err() << Err::msg_t("empty ontology IRI")
   );

   by_path_t& by_path = stor_.get<0>();
   path_iter_t pi = by_path.find(path);

   //check for duplicate locations
   if( pi != by_path.end() ) {
      //different ontology at same location
      if( pi->iri != iri || pi->version != version ) {
         BOOST_THROW_EXCEPTION(
               Err() << Err::msg_t("duplicate location") << Err::str1_t(path)
         );
      }
      //same ontology at same location -- do nothing
      return;
   }

   //check for duplicate non-empty version
   if( ! version.empty() ) {
      by_version_t& by_version = stor_.get<2>();
      version_iter_t vi = by_version.find(version);
      if( vi != by_version.end() )
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("duplicate version")
               << Err::str1_t(path)
               << Err::str2_t(vi->path)
               << Err::str3_t(version)
         );
   }

   //check for duplicate ID
   by_iri_t& by_iri = stor_.get<1>();
   iri_range_t iri_range = by_iri.equal_range(iri);
   BOOST_FOREACH(const Location& loc, iri_range) {
      if( loc.version == version ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("duplicate ontology")
               << Err::str1_t(path)
               << Err::str2_t(loc.path)
               << Err::str3_t(loc.iri)
         );
   }

   stor_.insert(pi, Location(path, iri, version));
}

/*
*******************************************************************************/
const std::string& Catalog::find_location(const std::string& iri) const {
   if( iri.empty() ) BOOST_THROW_EXCEPTION(
         Err() << Err::msg_t("empty ontology IRI")
   );

   //search versions
   const by_version_t& by_version = stor_.get<2>();
   version_iter_t vi = by_version.find(iri);
   if( vi != by_version.end() ) return vi->path;

   //search IRIs
   const by_iri_t& by_iri = stor_.get<1>();
   iri_iter_t ii = by_iri.find(iri);
   if( ii != by_iri.end() ) return ii->path;

   BOOST_THROW_EXCEPTION(
         Not_found_err()
         << Not_found_err::msg_t("not found")
         << Not_found_err::str1_t(iri)
   );
}

}//namespace owl_cpp
