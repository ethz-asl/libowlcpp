/** @file "/owlcpp/lib/io/adaptor_triple_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ADAPTOR_TRIPLE_STORE_HPP_
#define ADAPTOR_TRIPLE_STORE_HPP_
#include <deque>
#include "boost/assert.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/function.hpp"
#include "boost/array.hpp"

#include "raptor2.h"

#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/io/exception.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "adaptor_iri_finder.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Adaptor_triple_store {
   typedef boost::array<Node_id,3> triple_t;
   typedef std::deque<triple_t> triple_stor_t;
public:
   struct Err : public Input_err {};

   static std::string blank_name_prefix(const Doc_id did) {
      return "doc" + boost::lexical_cast<std::string>(did()) + "_";
   }

   /**
    Use this when path and ontology import IRI (ontologyIRI or versionIRI) for the
    document are known.
   */
   Adaptor_triple_store(
            Triple_store& ts, /**< destination triple store */
            std::string const& path, /**< document location (may be empty) */
            std::string const& import_iri = "" /**< expected versionIRI or ontologyIRI */
   ) :
      ts_(ts),
      current_doc_(get_doc_id(ts, path, import_iri)),
      blank_pref_(blank_name_prefix(current_doc_)),
      path_(path),
      import_iri_(import_iri),
      aif_(boost::bind(&Adaptor_triple_store::id_found, this)),
      id_found_(false)
   {}

   /**
    Use this when path, expected ontologyIRI, and expected versionIRI for the
    document that is going to be loaded are known exactly.
   */
   Adaptor_triple_store(
            Triple_store& ts, /**< destination triple store */
            std::string const& path, /**< document location (may be empty) */
            std::string const& ontology_iri, /**< expected ontologyIRI */
            std::string const& version_iri /**< expected versionIRI (may be empty) */
   ) :
      ts_(ts),
      current_doc_(get_doc_id(ts, path, ontology_iri, version_iri)),
      blank_pref_(blank_name_prefix(current_doc_)),
      path_(),
      import_iri_(),
      aif_(boost::bind(&Adaptor_triple_store::id_found, this)),
      id_found_(false)
   {}

   void insert(void const* statement) {
      if( ! id_found_ ) aif_.insert(statement);
      const raptor_statement* rs = static_cast<const raptor_statement*>(statement);
//      std::cout
//      << raptor_term_to_string(rs->subject) << ' '
//      << raptor_term_to_string(rs->predicate) << ' '
//      << raptor_term_to_string(rs->object) << '\n'
//      ;

      triple_t triple;
      triple[0] = insert_node(*rs->subject);
      triple[1] = insert_node(*rs->predicate);
      triple[2]  = insert_node(*rs->object);
      if( id_found_ ) {
         ts_.insert_triple(triple[0], triple[1], triple[2], current_doc_);
      } else {
         triples_.push_back(triple);
      }
   }

   const std::string& iri() const {return aif_.iri();}
   const std::string& version() const {return aif_.version();}
   void finalize() { if(! id_found_ ) id_found(); }

private:
   Triple_store& ts_;
   const Doc_id current_doc_;
   const std::string blank_pref_;
   const std::string path_;
   const std::string import_iri_;
   Adaptor_iri_finder aif_;
   bool id_found_;
   triple_stor_t triples_;

   void id_found() {
      check_iri();
      id_found_ = true;
   }

   /** Check whether expected ontology IRI is compatible with the declared IRIs.
   Add document info entry to triple store.
   */
   void check_iri() {
      const Node_id nid = ts_.documents().iri(current_doc_);
      if( nid == terms::T_empty_::id() ) { //document entry is incomplete
         if( ! import_iri_.empty() && import_iri_ != iri() && import_iri_ != version() ) {
            BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("ontology IRI mismatch")
                     << Err::str1_t(import_iri_)
                     << Err::str2_t(iri())
                     << Err::str3_t(version())
            );
         }
         ts_.documents().modify(
                  current_doc_,
                  path_,
                  ts_.insert_iri_node(iri()),
                  ts_.insert_iri_node(version())
         );
      } else { //document entry is complete
         Node_id const* iri_nid = ts_.find_iri_node(iri());
         if( ! iri_nid || *iri_nid != nid ) {
            BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("ontology IRI mismatch")
                     << Err::str1_t(ts_.string(nid))
                     << Err::str2_t(iri())
            );
         }
         const Node_id vid = ts_.documents().version(current_doc_);
         if( version() != ts_.string(vid) ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("ontology versionIRI mismatch")
                  << Err::str1_t(ts_.string(vid))
                  << Err::str2_t(version())
         );
      }
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
                     Err()
                     << Err::msg_t("unknown node type")
            );
      }
   }

   Node_id insert_node(raptor_uri* val) {
      std::size_t len;
      char const* str = reinterpret_cast<char const*>(
               raptor_uri_as_counted_string(val, &len)
      );
      return ts_.insert_iri_node(std::string(str, len));
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
      return ts_.insert_lit_node( std::string(val_str, val.string_len), type, lang );
   }

   Node_id insert_node(raptor_term_blank_value const& val) {
      char const* val_str = reinterpret_cast<char const*>(val.string);
      return ts_.insert_blank_node( blank_pref_ + std::string(val_str,val.string_len) );
   }

   static Doc_id get_doc_id(
            Triple_store& ts, /**< destination triple store */
            std::string const& path, /**< document location (may be empty) */
            std::string const& import_iri /**< expected ontologyIRI */
   ) {
      if( ! path.empty() && ts.documents().find_path(path) )
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("document already loaded")
                  << Err::str1_t(path)
         );
      if( ! import_iri.empty() && ts.find_doc_iri(import_iri) ) {
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("document already loaded")
                  << Err::str1_t(import_iri)
         );
      }
      return ts.documents().insert_new();
   }

   static Doc_id get_doc_id(
            Triple_store& ts, /**< destination triple store */
            std::string const& path, /**< document location (may be empty) */
            std::string const& ontology_iri, /**< expected ontologyIRI */
            std::string const& version_iri /**< expected versionIRI (may be empty) */
   ) {
      if( ontology_iri.empty() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("valid ontologyIRI is required")
      );
      std::pair<Doc_id,bool> p = ts.insert_doc(path, ontology_iri, version_iri);
      if( ! p.second ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("document already loaded")
      << Err::str1_t(ontology_iri)
      );
      return p.first;
   }
};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_TRIPLE_STORE_HPP_ */
