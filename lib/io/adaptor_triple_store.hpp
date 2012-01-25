/** @file "/owlcpp/lib/io/adaptor_triple_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ADAPTOR_TRIPLE_STORE_HPP_
#define ADAPTOR_TRIPLE_STORE_HPP_
#include "boost/assert.hpp"

#include "raptor2.h"

#include "boost/function.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Adaptor_triple_store {
public:
   struct Err : public base_exception {};
   Adaptor_triple_store(Triple_store& ts) : ts_(ts), current_doc_() {}

   void insert(void const* statement, boost::function<void()> abort) {
      const raptor_statement* rs = static_cast<const raptor_statement*>(statement);
//      std::cout
//      << raptor_term_to_string(rs->subject) << ' '
//      << raptor_term_to_string(rs->predicate) << ' '
//      << raptor_term_to_string(rs->object) << '\n'
//      ;


   }

   void current_doc(const Doc_id did) { current_doc_ = did; }

private:
   Triple_store& ts_;
   Doc_id current_doc_;

   Node_id insert_node(raptor_term const& node) {
      std::size_t len;
      char const* str = reinterpret_cast<char const*>(
//               raptor_uri_as_counted_string(node->value.uri, &len)
      );
   }
};

}//namespace detail
}//namespace owlcpp
#endif /* ADAPTOR_TRIPLE_STORE_HPP_ */
