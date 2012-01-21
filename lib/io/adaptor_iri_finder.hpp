/** @file "/owlcpp/lib/io/adaptor_iri_finder.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ADAPTOR_IRI_FINDER_HPP_
#define ADAPTOR_IRI_FINDER_HPP_

#include "raptor2.h"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Iri_finder {
public:
   Iri_finder() : iri_(), version_(), stop_parsing_(false) {}

   bool stop_parsing() const {return stop_parsing_;}

   void insert(raptor_statement const* rs) {
/*
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

   static const std::string& owl_Ontology() {
      static const std::string s = full_name(ot::T_owl_Ontology());
      return s;
   }
   static const std::string& rdf_type() {
      static const std::string s = full_name(ot::T_rdf_type());
      return s;
   }
   static const std::string& owl_versionIRI() {
      static const std::string s = full_name(ot::T_owl_versionIRI());
      return s;
   }
};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_IRI_FINDER_HPP_ */
