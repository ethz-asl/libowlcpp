/** @file "/owlcpp/lib/io/adaptor_triple_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ADAPTOR_TRIPLE_STORE_HPP_
#define ADAPTOR_TRIPLE_STORE_HPP_
#include "boost/assert.hpp"
#include "boost/lexical_cast.hpp"

#include "raptor2.h"

#include "boost/function.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/io/exception.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Adaptor_triple_store {
public:
   struct Err : public Input_err {};
   Adaptor_triple_store(Triple_store& ts) : ts_(ts), current_doc_() {}

   void insert(void const* statement, boost::function<void()> abort) {
      const raptor_statement* rs = static_cast<const raptor_statement*>(statement);
//      std::cout
//      << raptor_term_to_string(rs->subject) << ' '
//      << raptor_term_to_string(rs->predicate) << ' '
//      << raptor_term_to_string(rs->object) << '\n'
//      ;
      const Node_id subj = insert_node(*rs->subject);
      const Node_id pred = insert_node(*rs->predicate);
      const Node_id obj  = insert_node(*rs->object);
      ts_.insert_triple(subj, pred, obj, current_doc_);
   }

   void current_doc(const Doc_id did) {
      current_doc_ = did;
      doc_str_ = "Doc" + boost::lexical_cast<std::string>(did()) + "_";
   }

private:
   Triple_store& ts_;
   Doc_id current_doc_;
   std::string doc_str_;

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
      return ts_.insert_blank_node( doc_str_ + std::string(val_str,val.string_len) );
   }
};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_TRIPLE_STORE_HPP_ */
