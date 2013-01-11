/** @file "/owlcpp/lib/io/raptor_to_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RAPTOR_TO_IRI_HPP_
#define RAPTOR_TO_IRI_HPP_
#include "boost/assert.hpp"
#include "boost/function.hpp"

#include "raptor2.h"

#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/io/exception.hpp"

namespace owlcpp{ namespace detail{

/**@brief Find ontologyIRI and versionIRI from Raptor parser statements
*******************************************************************************/
class Raptor_to_iri {
public:
   typedef Input_err Err;

   Raptor_to_iri(
            const boost::function<void()> abort_call,
            const std::size_t search_depth = std::numeric_limits<std::size_t>::max()
   )
   : iri_(),
     version_(),
     abort_call_(abort_call),
     max_depth_(search_depth),
     n_(0)
   {}

   void insert(void const* statement) {
      const raptor_statement* rs = static_cast<const raptor_statement*>(statement);

      if( iri_.empty() && is_ontologyIRI(*rs) ) {
         std::size_t len;
         unsigned char const* term_uc =
                  raptor_uri_as_counted_string(rs->subject->value.uri, &len);
         char const* term = reinterpret_cast<char const*>(term_uc);
         iri_.assign(term, len);

      } else if( is_versionIRI(*rs, iri_) ) {
         std::size_t len;
         unsigned char const* term_uc =
                  raptor_uri_as_counted_string(rs->object->value.uri, &len);
         char const* term = reinterpret_cast<char const*>(term_uc);
         version_.assign(term, len);
         abort_call_();

      } else if( max_depth_ == n_++ ) {
      //Early termination of the search for versionIRI triple
      // to avoid reading large ontologies.
         abort_call_();
      }

   }

   const std::string& iri() const {return iri_;}
   const std::string& version() const {return version_;}
   void finalize() const {}

private:
   std::string iri_;
   std::string version_;
   const boost::function<void()> abort_call_;
   const std::size_t max_depth_;
   std::size_t n_;

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
      if( ! comparison(term, len, terms::owl_Ontology()) ) return false;

      term_uc =
               raptor_uri_as_counted_string(rs.predicate->value.uri, &len);
      term = reinterpret_cast<char const*>(term_uc);
      return comparison(term, len, terms::rdf_type());
   }

   static bool is_versionIRI(raptor_statement const& rs, std::string const& iri) {
      if( iri.empty() ) return false;
      if(
               rs.subject->type != RAPTOR_TERM_TYPE_URI ||
               rs.predicate->type != RAPTOR_TERM_TYPE_URI ||
               rs.object->type != RAPTOR_TERM_TYPE_URI
      ) return false;

      std::size_t len;
      unsigned char const* term_uc =
               raptor_uri_as_counted_string(rs.predicate->value.uri, &len);
      char const* term = reinterpret_cast<char const*>(term_uc);
      if( ! comparison(term, len, terms::owl_versionIRI()) ) return false;

      term_uc =
               raptor_uri_as_counted_string(rs.subject->value.uri, &len);
      term = reinterpret_cast<char const*>(term_uc);
      if( iri.compare(0, iri.size(), term, 0, len) != 0 ) {
         const std::string iri_term(term, len);
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("invalid versionIRI statement")
                  << Err::str1_t(iri_term)
                  << Err::str2_t(iri)
         );
      }
      return true;
   }
};
}//namespace detail
}//namespace owlcpp
#endif /* RAPTOR_TO_IRI_HPP_ */
