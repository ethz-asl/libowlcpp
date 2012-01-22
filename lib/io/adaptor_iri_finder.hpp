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

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Iri_finder {
public:
   Iri_finder() : iri_(), version_(), stop_parsing_(false) {}

   bool stop_parsing() const {return stop_parsing_;}

   void insert(void const* statement) {
      const raptor_statement* rs = static_cast<const raptor_statement*>(statement);
      std::cout
      << raptor_term_to_string(rs->subject) << ' '
      << raptor_term_to_string(rs->predicate) << ' '
      << raptor_term_to_string(rs->object) << '\n'
      ;
      if(
               rs->subject->type != RAPTOR_TERM_TYPE_URI ||
               rs->predicate->type != RAPTOR_TERM_TYPE_URI ||
               rs->object->type != RAPTOR_TERM_TYPE_URI
      ) return;
      std::size_t obj_len;
      unsigned char const* obj =
               raptor_uri_as_counted_string(rs->object->value.uri, &obj_len);
      if( ! comparison(reinterpret_cast<char const*>(obj), obj_len, terms::T_owl_Ontology()) ) return;

      std::size_t pred_len;
      unsigned char const* pred =
               raptor_uri_as_counted_string(rs->predicate->value.uri, &pred_len);
/*
      if(
               rs->predicate->type == RAPTOR_TERM_TYPE_URI &&
               comparison(
//                        raptor_uri_as_counted_string(rs->predicate->value.uri->string,
                        rs->predicate->value.uri->length,
                        terms::T_owl_Ontology()
               )
      ) {

      }
      if(
            triple.get<2>().type == Resource &&
            triple.get<2>().value == owl_Ontology() &&
            triple.get<1>().type == Resource &&
            triple.get<1>().value == rdf_type()
      ) {
         assert( triple.get<0>().type == Resource );
         iri_ = triple.get<0>().value;
      } else if(
            triple.get<1>().type == Resource &&
            triple.get<1>().value == owl_versionIRI()
      ) {
         assert( triple.get<0>().type == Resource );
         if( triple.get<0>().value != iri_ ) BOOST_THROW_EXCEPTION(
               Parse_err()
               << Parse_err::msg_t("ontology IRI mismatch")
               << Parse_err::str1_t(triple.get<0>().value)
               << Parse_err::str2_t(iri_)
            );
         version_ = triple.get<2>().value;
         stop_parsing_ = true;
      } else if( ! iri_.empty() && triple.get<0>().value != iri_ ) {
         //Most ontologies have no version IRI.
         //To avoid parsing entire ontology searching for version IRI
         //take a short-cut.
         //Assume there is only one owl:Ontology element.
         //If iri_ is found and the subject of current triple is not iri_,
         //we must be outside of owl:Ontology element and may stop searching for version.
         //TODO: Make sure this is actually a good idea.
         stop_parsing_ = true;
      }
*/
   }

   const std::string& iri() const {return iri_;}
   const std::string& version() const {return version_;}

private:
   std::string iri_;
   std::string version_;
   bool stop_parsing_;

};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_IRI_FINDER_HPP_ */
