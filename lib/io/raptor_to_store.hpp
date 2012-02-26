/** @file "/owlcpp/lib/io/raptor_to_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RAPTOR_TO_STORE_HPP_
#define RAPTOR_TO_STORE_HPP_
#include <string>
#include <deque>
#include "boost/bind.hpp"
#include "boost/function.hpp"

#include "owlcpp/io/exception.hpp"
#include "raptor_to_iri.hpp"
#include "triple_store_temp.hpp"
#include "raptor_wrapper.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/copy_triples.hpp"

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
   Raptor_to_store(
            Triple_store& ts,
            std::string const& path = "",
            Ontology_id_checker const& checker = No_check()
   )
   : ts_(ts),
     parser_(),
     abort_call_(parser_.abort_call()),
     checker_(checker),
     path_(path),
     rti_(boost::bind(&Raptor_to_store::id_found, this)),
     id_found_(false)
   {}

   /*
    TODO: Make temporary storage for IRIs, nodes, and triples
    Move them to main triple store after parsing is complete.
    IRI IDs, node IDs, and blank node names will have to be translated.
    */
   void insert(void const* statement) {
/*
      if( ! id_found_ ) aif_.insert(statement);
      raptor_statement const& rs = *static_cast<raptor_statement const*>(statement);
      check_import(rs);
      insert_triple(tst_, rs);
*/
   }

   const std::string& iri() const {return rti_.iri();}
   const std::string& version() const {return rti_.version();}
   std::deque<std::string> const& imports() const {return imports_;}

//   void finalize() {
//      copy_triples(tst_, ts_);
//   }

   void parse() {

   }

   void parse(std::istream& stream) {
      parser_(stream, *this);
      copy_triples(tst_, ts_);
   }

private:
   Triple_store& ts_;
   Raptor_wrapper parser_;
   const boost::function<void()> abort_call_;
   Ontology_id_checker const& checker_;
   const std::string path_;
   Raptor_to_iri rti_;
   bool id_found_;
   Triple_store_temp tst_;
   std::deque<std::string> imports_;

   void id_found() {
      try{
         checker_(rti_.iri(), rti_.version());
         check_loaded( rti_.iri(), rti_.version() );
      } catch(Ontology_id_checker::Err e) {
         abort_call_();
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("error parsing")
                  << Err::str1_t(path_)
                  << Err::nested_t(boost::current_exception())
         );
      }
      id_found_ = true;
   }

   void check_import(raptor_statement const& rs) {
      if(
               rs.subject->type != RAPTOR_TERM_TYPE_URI ||
               rs.predicate->type != RAPTOR_TERM_TYPE_URI ||
               rs.object->type != RAPTOR_TERM_TYPE_URI
      ) return;

      std::size_t len;
      unsigned char const* term_uc =
               raptor_uri_as_counted_string(rs.predicate->value.uri, &len);
      char const* term = reinterpret_cast<char const*>(term_uc);
      if( ! comparison(term, len, terms::T_owl_imports()) ) return;

      term_uc = raptor_uri_as_string(rs.subject->value.uri);
      term = reinterpret_cast<char const*>(term_uc);
      if( iri() != term ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid subject in imports triple")
               << Err::str1_t(iri())
               << Err::str2_t(term)
      );

      term_uc = raptor_uri_as_counted_string(rs.object->value.uri, &len);
      term = reinterpret_cast<char const*>(term_uc);
      imports_.push_back(std::string(term, len));
   }

   void check_loaded(std::string const& iri, std::string const& version) const {
      if( ! version.empty() && ts_.find_doc_iri(version) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("versionIRI already loaded")
               << Err::str1_t(version)
      );
      if( ts_.find_doc_iri(iri) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontologyIRI already loaded")
               << Err::str1_t(iri)
      );
   }

};

}//namespace detail
}//namespace owlcpp
#endif /* RAPTOR_TO_STORE_HPP_ */
