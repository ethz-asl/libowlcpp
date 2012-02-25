/** @file "/owlcpp/lib/io/raptor_to_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RAPTOR_TO_STORE_HPP_
#define RAPTOR_TO_STORE_HPP_
#include <string>
#include "boost/bind.hpp"

#include "owlcpp/io/exception.hpp"
#include "raptor_to_iri.hpp"
#include "triple_store_temp.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
struct Ontology_id_checker {
   struct Err : public Input_err {};
   virtual void operator()(std::string const& iri, std::string const& ver) const = 0;
   virtual ~Ontology_id_checker() {}
};

/**@brief
*******************************************************************************/
struct No_check : public Ontology_id_checker {
   void operator()(std::string const & iri, std::string const&) const {
      if( iri.empty() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("empty ontologyIRI")
      );
   }
};

/**@brief
*******************************************************************************/
class Check_iri : public No_check {
public:
   Check_iri(std::string const& iri) : iri_(iri) {}

   void operator()(std::string const& iri, std::string const& ver) const {
      No_check::operator ()(iri, ver);
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

/**@brief
*******************************************************************************/
class Check_version : public No_check {
public:
   Check_version(std::string const& iri, std::string const& ver)
   : iri_(iri), ver_(ver) {}

   void operator()(std::string const& iri, std::string const& ver) const {
      No_check::operator ()(iri, ver);
      if( iri_ != iri ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontology IRI mismatch")
               << Err::str1_t(iri)
               << Err::str3_t(iri_)
      );
      if( ver_ != ver ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontology versionIRI mismatch")
               << Err::str2_t(ver)
               << Err::str3_t(ver_)
      );
   }
private:
   const std::string iri_;
   const std::string ver_;
};

/**@brief
*******************************************************************************/
class Raptor_to_store {
public:
   typedef Input_err Err;
   Raptor_to_store( std::string const& path, Ontology_id_checker const& checker)
   : checker_(checker),
     path_(path),
     rti_(boost::bind(&Raptor_to_store::id_found, this)),
     id_found_(false)
   {}

private:
   Ontology_id_checker const& checker_;
   const std::string path_;
   Raptor_to_iri rti_;
   bool id_found_;
   Triple_store_temp tst_;
   Triple_store& ts_;

   void id_found() {

   }
};

}//namespace detail
}//namespace owlcpp
#endif /* RAPTOR_TO_STORE_HPP_ */
