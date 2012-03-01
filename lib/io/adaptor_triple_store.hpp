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
#include "boost/foreach.hpp"

#include "raptor2.h"

#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/io/exception.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "raptor_to_iri.hpp"
#include "triple_store_temp.hpp"

namespace owlcpp{ namespace detail{

/**
*******************************************************************************/
template<class TS> inline Node_id insert_node(TS& ts, raptor_uri* val) {
   std::size_t len;
   char const* str = reinterpret_cast<char const*>(
            raptor_uri_as_counted_string(val, &len)
   );
   return ts.insert_iri_node(std::string(str, len));
}

/**
*******************************************************************************/
template<class TS> inline Node_id insert_node(TS& ts, raptor_term_literal_value const& val) {
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
   return ts.insert_lit_node( std::string(val_str, val.string_len), type, lang );
}

/**
*******************************************************************************/
template<class TS> inline Node_id insert_node(
         TS& ts, raptor_term_blank_value const& val, const Doc_id did
) {
   char const* val_str = reinterpret_cast<char const*>(val.string);
   return ts.insert_blank_node(did, std::string(val_str, val.string_len));
}

/**
*******************************************************************************/
template<class TS> inline Node_id insert_node(TS& ts, raptor_term const& node, const Doc_id did) {
   switch (node.type) {
   case RAPTOR_TERM_TYPE_URI:
      return insert_node(ts, node.value.uri);
   case RAPTOR_TERM_TYPE_LITERAL:
      return insert_node(ts, node.value.literal);
   case RAPTOR_TERM_TYPE_BLANK:
      return insert_node(ts, node.value.blank, did);
   default:
      BOOST_THROW_EXCEPTION(
               Input_err()
               << Input_err::msg_t("unknown node type")
         );
   }
}

/**
*******************************************************************************/
template<class TS> inline void insert_triple(
         TS& ts, raptor_statement const& rs, const Doc_id did
) {
   const Node_id subj = insert_node(ts, *rs.subject, did);
   const Node_id pred = insert_node(ts, *rs.predicate, did);
   const Node_id obj = insert_node(ts, *rs.object, did);
   ts.insert_triple(subj, pred, obj, did);
}


/**@brief Accept RDF triples from a parser
*******************************************************************************/
class Adaptor_triple_store {
public:
   struct Err : public Input_err {};

   /**
    Use this when path and ontology import IRI (ontologyIRI or versionIRI) for the
    document are known.
   */
   Adaptor_triple_store(
            std::string const& path, /**< document location (may be empty) */
            std::string const& import_iri = "" /**< expected versionIRI or ontologyIRI */
   ) :
      path_(path),
      expected_iri_(import_iri),
      aif_(boost::bind(&Adaptor_triple_store::id_found, this)),
      id_found_(false)
   {}

   /**
    Use this when path, expected ontologyIRI, and expected versionIRI for the
    document that is going to be loaded are known exactly.
   */
   Adaptor_triple_store(
            std::string const& path, /**< document location (may be empty) */
            std::string const& ontology_iri, /**< expected ontologyIRI */
            std::string const& version_iri /**< expected versionIRI (may be empty) */
   ) :
      path_(),
      expected_iri_(),
      aif_(boost::bind(&Adaptor_triple_store::id_found, this)),
      id_found_(false)
   {}

   /*
    TODO: Make temporary storage for IRIs, nodes, and triples
    Move them to main triple store after parsing is complete.
    IRI IDs, node IDs, and blank node names will have to be translated.
    */
   void insert(void const* statement) {
      if( ! id_found_ ) aif_.insert(statement);
      raptor_statement const& rs = *static_cast<raptor_statement const*>(statement);
      check_import(rs);
      insert_triple(tst_, rs);
   }

   const std::string& iri() const {return aif_.iri();}
   const std::string& version() const {return aif_.version();}
   void finalize() { if(! id_found_ ) id_found(); }
   std::deque<std::string> const& imports() const {return imports_;}

private:
   Triple_store_temp tst_;
   const std::string path_;
   const std::string expected_iri_;
   Adaptor_iri_finder aif_;
   bool id_found_;
   std::deque<std::string> imports_;

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
               << Err::msg_t("invalid import statement")
               << Err::str1_t(iri())
               << Err::str2_t(term)
      );

      term_uc = raptor_uri_as_counted_string(rs.object->value.uri, &len);
      term = reinterpret_cast<char const*>(term_uc);
      imports_.push_back(std::string(term, len));
   }

   void id_found() {
      check_iri();
      BOOST_FOREACH( triple_t const& t, triples_) {
         ts_.insert_triple(t[0], t[1], t[2], current_doc_);
      }
      triples_.clear();
      id_found_ = true;
   }

   /** Check whether expected ontology IRI is compatible with the declared IRIs.
   Add document info entry to triple store.
   */
   void check_iri() {
      const Node_id nid = tst_.ontology_iri();
      if( is_empty(nid) ) { //document entry is incomplete
         if(
                  ! expected_iri_.empty() &&
                  expected_iri_ != iri() &&
                  expected_iri_ != version()
         ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("ontology IRI mismatch")
                  << Err::str1_t(expected_iri_)
                  << Err::str2_t(iri())
                  << Err::str3_t(version())
         );
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
         const Node_id vid = ts_.documents().version_iri(current_doc_);
         if( version() != ts_.string(vid) ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("ontology versionIRI mismatch")
                  << Err::str1_t(ts_.string(vid))
                  << Err::str2_t(version())
         );
      }
   }

};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_TRIPLE_STORE_HPP_ */
