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
#include "boost/filesystem/fstream.hpp"

#include "owlcpp/io/exception.hpp"
#include "raptor_to_iri.hpp"
#include "triple_store_temp.hpp"
#include "owlcpp/io/raptor_wrapper.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/copy_triples.hpp"

namespace owlcpp{ namespace detail{

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
            Check_id const& checker = Check_id()
   )
   : ts_(ts),
     parser_(),
     abort_call_(parser_.abort_call()),
     checker_(checker),
     path_(path),
     rti_(boost::bind(&Raptor_to_store::id_found, this)),
     id_found_(false)
   {}

   void insert(void const* statement) {
      if( ! id_found_ ) rti_.insert(statement);
      raptor_statement const& rs = *static_cast<raptor_statement const*>(statement);
      check_import(rs);
      const Node_id subj = insert_node(*rs.subject);
      const Node_id pred = insert_node(*rs.predicate);
      const Node_id obj = insert_node(*rs.object);
      tst_.insert_triple(subj, pred, obj);
   }

   const std::string& iri() const {return rti_.iri();}
   const std::string& version() const {return rti_.version();}
   std::deque<std::string> const& imports() const {return imports_;}

   void parse() {
      boost::filesystem::ifstream ifs(path_);
      parse(ifs);
   }

   void parse(std::istream& stream) {
      parser_(stream, *this);
      if( ! id_found_ ) id_found();
      copy_triples(tst_, ts_);
   }

private:
   Triple_store& ts_;
   Raptor_wrapper parser_;
   const boost::function<void()> abort_call_;
   Check_id const& checker_;
   const std::string path_;
   Raptor_to_iri rti_;
   bool id_found_;
   Triple_store_temp tst_;
   std::deque<std::string> imports_;

   /** This method is called when ontologyIRI and (possibly) versionIRI statements
    are encountered during parsing.
    Check ontologyIRI and versionIRI with instance of Check_id.
    Check if ontology with same ID has already been loaded.
    Set ontologyIRI and versionIRI to temporary triple store.
   */
   void id_found() {
      try{
         checker_(rti_.iri(), rti_.version());
         check_loaded( rti_.iri(), rti_.version() );
         tst_.set_ids(rti_.iri(), rti_.version());
      } catch(Check_id::Err e) {
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

   /** If @b rs is an import triple, check its validity and store the importsIRI.
    @param rs Raptor-style RDF triple
   */
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

   /**
    @param iri ontologyIRI
    @param version versionIRI
    @throw Err if ontology with same ontologyIRI or same non-empty versionIRI
    has already been loaded
   */
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

   Node_id insert_node(raptor_term const& node) {
      switch (node.type) {
      case RAPTOR_TERM_TYPE_URI:
         return insert_node(node.value.uri);
      case RAPTOR_TERM_TYPE_LITERAL:
         return insert_node(node.value.literal);
      case RAPTOR_TERM_TYPE_BLANK:
         return insert_node(node.value.blank);
      default:
         BOOST_THROW_EXCEPTION(
                  Input_err()
                  << Input_err::msg_t("unknown node type")
            );
      }
   }

   Node_id insert_node(raptor_term_blank_value const& val) {
      char const* val_str = reinterpret_cast<char const*>(val.string);
      return tst_.insert_blank_node(std::string(val_str, val.string_len));
   }

   Node_id insert_node(raptor_term_literal_value const& val) {
      std::string type;
      if( val.datatype ) {
         std::size_t len;
         char const* str = reinterpret_cast<char const*>(
                  raptor_uri_as_counted_string(val.datatype, &len)
         );
         type.assign(str, len);
      }

      char const* lang_str = reinterpret_cast<char const*>(val.language);
      const std::string lang =
               val.language ? std::string(lang_str, val.language_len) : "";

      char const* val_str = reinterpret_cast<char const*>(val.string);
      return tst_.insert_lit_node( std::string(val_str, val.string_len), type, lang );
   }

   Node_id insert_node(raptor_uri* val) {
      std::size_t len;
      char const* str = reinterpret_cast<char const*>(
               raptor_uri_as_counted_string(val, &len)
      );
      return tst_.insert_iri_node(std::string(str, len));
   }

};

}//namespace detail
}//namespace owlcpp
#endif /* RAPTOR_TO_STORE_HPP_ */
