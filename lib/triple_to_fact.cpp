/** @file "/owl_cpp/lib/triple_to_fact.cpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
//#include "pch.hpp"
#include <iostream>
#include "boost/foreach.hpp"
#include "boost/exception/get_error_info.hpp"
namespace b = boost;

#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/triple_to_fact.hpp"
#include "owl_cpp/query_nodes.hpp"
#include "owl_cpp/print.hpp"
#include "owl_cpp/exception.hpp"
#include "fact_visitor.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp {
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
}//namespace owl_cpp