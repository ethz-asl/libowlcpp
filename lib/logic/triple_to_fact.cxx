/** @file "/owlcpp/lib/reasoner/triple_to_fact.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef OWLCPP_REASONER_SOURCE
#define OWLCPP_REASONER_SOURCE
#endif
#include "owlcpp/reasoner/triple_to_fact.hpp"

#include <iostream>
#include "boost/foreach.hpp"
#include "boost/exception/get_error_info.hpp"
namespace b = boost;

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_nodes.hpp"
#include "owlcpp/print.hpp"
#include "owlcpp/exception.hpp"
#include "fact_visitor.hpp"
#include "owlcpp/terms/term_tags.hpp"
namespace ot = owlcpp::terms;

namespace owlcpp {
/*
*******************************************************************************/
void add(const Triple_store& store, ReasoningKernel& kernel, const bool lax) {
   Triple_to_fact ttf(store, kernel);
   BOOST_FOREACH(const Triple& triple, store.triples()) {
      try {
         triple.Accept(ttf);
      } catch(Triple_to_fact::Warn& e) {
         if( lax ) {
            std::cerr << "Warning! ";
            if( const std::string* msg = b::get_error_info<Triple_to_fact::Warn::msg_t>(e) )
               std::cerr << *msg;
            std::cerr << " in ";
            print(triple, store, std::cerr, "\n");
         } else {
            BOOST_THROW_EXCEPTION(
                  base_exception()
                  << base_exception::msg_t("error adding to reasoner")
                  << base_exception::str1_t(print(triple, store))
                  << base_exception::nested_t(b::current_exception())
            );
         }
      } catch(...) {
         BOOST_THROW_EXCEPTION(
               base_exception()
               << base_exception::msg_t("error adding to reasoner")
               << base_exception::str1_t(print(triple, store))
               << base_exception::nested_t(b::current_exception())
         );
      }
   }
}

/*
*******************************************************************************/
}//namespace owlcpp
