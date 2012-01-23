/** @file "/owlcpp/lib/io/adaptor_iri_finder.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ADAPTOR_IRI_FINDER_HPP_
#define ADAPTOR_IRI_FINDER_HPP_
#include "boost/assert.hpp"

#include "raptor2.h"

#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Iri_finder {
public:
   struct Err : public base_exception {};
   Iri_finder() : iri_(), version_(), stop_parsing_(false) {}

   bool stop_parsing() const {return stop_parsing_;}

   void insert(void const* statement) {
      const raptor_statement* rs = static_cast<const raptor_statement*>(statement);
//      std::cout
//      << raptor_term_to_string(rs->subject) << ' '
//      << raptor_term_to_string(rs->predicate) << ' '
//      << raptor_term_to_string(rs->object) << '\n'
//      ;

      if( iri_.empty() && is_ontologyIRI(*rs) ) {
         std::size_t len;
         unsigned char const* term_uc =
                  raptor_uri_as_counted_string(rs->subject->value.uri, &len);
         char const* term = reinterpret_cast<char const*>(term_uc);
         iri_.assign(term, len);
         return;
      }

      if( is_versionIRI(*rs) ) {
         std::size_t len;
         unsigned char const* term_uc =
                  raptor_uri_as_counted_string(rs->object->value.uri, &len);
         char const* term = reinterpret_cast<char const*>(term_uc);
         version_.assign(term, len);
         stop_parsing_ = true;
         return;
      }

      //TODO: Implement early termination of the search for versionIRI statement
      // to avoid reading large ontologies.
      // This is particularly important since most ontologies do not have versionIRIs.
      // E.g., terminate search 1000 triples after last OntologyIRI IRI appeared
      // in the subject.

   }

   const std::string& iri() const {return iri_;}
   const std::string& version() const {return version_;}

private:
   std::string iri_;
   std::string version_;
   bool stop_parsing_;

   static bool is_ontologyIRI(raptor_statement const& rs) {
      if(
               rs.subject->type != RAPTOR_TERM_TYPE_URI ||
               rs.predicate->type != RAPTOR_TERM_TYPE_URI ||
               rs.object->type != RAPTOR_TERM_TYPE_URI
      ) return false;
      std::size_t len;
      unsigned char const* term_uc =
               raptor_uri_as_counted_string(rs.object->value.uri, &len);
      char const* term = reinterpret_cast<char const*>(term_uc);
      if( ! comparison(term, len, terms::T_owl_Ontology()) ) return false;

      term_uc =
               raptor_uri_as_counted_string(rs.predicate->value.uri, &len);
      term = reinterpret_cast<char const*>(term_uc);
      return comparison(term, len, terms::T_rdf_type());
   }

   bool is_versionIRI(raptor_statement const& rs) const {
      if( iri_.empty() ) return false;
      if(
               rs.subject->type != RAPTOR_TERM_TYPE_URI ||
               rs.predicate->type != RAPTOR_TERM_TYPE_URI ||
               rs.object->type != RAPTOR_TERM_TYPE_URI
      ) return false;

      std::size_t len;
      unsigned char const* term_uc =
               raptor_uri_as_counted_string(rs.predicate->value.uri, &len);
      char const* term = reinterpret_cast<char const*>(term_uc);
      if( ! comparison(term, len, terms::T_owl_versionIRI()) ) return false;

      term_uc =
               raptor_uri_as_counted_string(rs.subject->value.uri, &len);
      term = reinterpret_cast<char const*>(term_uc);
      if( iri_.compare(0, iri_.size(), term, 0, len) != 0 ) {
         const std::string iri_term(term, len);
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("invalid versionIRI statement")
                  << Err::str1_t(iri_term)
                  << Err::str2_t(iri_)
         );
      }
      return true;
   }

};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_IRI_FINDER_HPP_ */
