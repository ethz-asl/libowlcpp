/** @file "/owlcpp/include/owlcpp/io/check_ontology_id.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CHECK_ONTOLOGY_ID_HPP_
#define CHECK_ONTOLOGY_ID_HPP_
#include <string>
#include "owlcpp/io/exception.hpp"

namespace owlcpp{

/** Check ontologyIRI and versionIRI during parsing.
 @throw Err if @b iri is empty
*******************************************************************************/
struct Check_id {
   struct Err : public Input_err {};
   virtual void operator()(std::string const& iri, std::string const&) const {
      if( iri.empty() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("empty ontologyIRI")
      );
   }
   virtual ~Check_id() {}
};

/**
*******************************************************************************/
struct No_check : public Check_id {
   void operator()(std::string const& , std::string const&) const {}
};

/** Check ontologyIRI and versionIRI during parsing.
 @throw Err if expected IRI does not match @b iri and @b ver
*******************************************************************************/
class Check_iri : public Check_id {
public:
   Check_iri(std::string const& iri) : iri_(iri) {}

   void operator()(std::string const& iri, std::string const& ver) const {
      Check_id::operator ()(iri, ver);
      if( iri_ != iri && iri_ != ver ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontology IRI mismatch")
               << Err::str1_t(iri)
               << Err::str2_t(ver)
               << Err::str3_t(iri_)
      );
   }
private:
   const std::string iri_;
};

/** Check ontologyIRI and versionIRI during parsing.
 @throw Err if @b iri does not match expected ontologyIRI or
 @b ver does not match expected versionIRI
*******************************************************************************/
class Check_both : public Check_id {
public:
   Check_both(std::string const& iri, std::string const& ver)
   : iri_(iri), ver_(ver) {}

   void operator()(std::string const& iri, std::string const& ver) const {
      Check_id::operator ()(iri, ver);
      if( iri_ != iri ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontology IRI mismatch")
               << Err::str1_t(iri)
               << Err::str2_t(iri_)
      );
      if( ver_ != ver ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontology versionIRI mismatch")
               << Err::str1_t(ver)
               << Err::str2_t(ver_)
      );
   }
private:
   const std::string iri_;
   const std::string ver_;
};

}//namespace owlcpp
#endif /* CHECK_ONTOLOGY_ID_HPP_ */
