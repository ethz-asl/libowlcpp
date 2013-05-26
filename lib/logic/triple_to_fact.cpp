/** @file "/owlcpp/lib/logic/triple_to_fact.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef OWLCPP_LOGIC_SOURCE
#define OWLCPP_LOGIC_SOURCE
#endif
#include "owlcpp/logic/triple_to_fact.hpp"
#include "boost/foreach.hpp"
#include "factpp/Kernel.hpp"
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_triple.hpp"

namespace owlcpp {

/*
*******************************************************************************/
TDLAxiom* submit(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict
) {
   logic::factpp::Adaptor_triple at(ts, kernel, strict);
   try{
      return at.submit(t);
   } catch(Logic_err const&) {
      BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("error submitting triple")
               << Logic_err::str1_t(to_string(t, ts))
               << Logic_err::str2_t(ts[t.doc_].path)
               << Logic_err::nested_t(boost::current_exception())
      );
   }
}

/*
*******************************************************************************/
TDLAxiom* submit_check(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict
) {
   logic::factpp::Adaptor_triple at(ts, kernel, strict);
   TDLAxiom* a = 0;
   try{
      a = at.submit(t);
   } catch(Logic_err const&) {
      BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("error submitting triple")
               << Logic_err::str1_t(to_string(t, ts))
               << Logic_err::str2_t(ts[t.doc_].path)
               << Logic_err::nested_t(boost::current_exception())
      );
   }
   if( a ) try{
      kernel.isKBConsistent();
   }catch(...) {
      BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("reasoning error after submitting triple")
               << Logic_err::str1_t(to_string(t, ts))
               << Logic_err::str2_t(ts[t.doc_].path)
               << Logic_err::nested_t(boost::current_exception())
      );
   }
   return a;
}

/*
*******************************************************************************/
std::size_t submit(
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict
) {
   std::size_t n = 0;
   logic::factpp::Adaptor_triple at(ts, kernel, strict);
   BOOST_FOREACH(Triple const& t, ts.map_triple()) {
      try{
         if( at.submit(t) ) ++n;
      } catch(Logic_err const&) {
         BOOST_THROW_EXCEPTION(
                  Logic_err()
                  << Logic_err::msg_t("error submitting triple")
                  << Logic_err::str1_t(to_string(t, ts))
                  << Logic_err::str2_t(ts[t.doc_].path)
                  << Logic_err::nested_t(boost::current_exception())
         );
      }
   }
   return n;
}

/*
*******************************************************************************/
std::size_t submit_check(
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict
) {
   std::size_t n = 0;
   logic::factpp::Adaptor_triple at(ts, kernel, strict);
   BOOST_FOREACH(Triple const& t, ts.map_triple()) {
      TDLAxiom* a = 0;
      try{
         a = at.submit(t);
      } catch(Logic_err const&) {
         BOOST_THROW_EXCEPTION(
                  Logic_err()
                  << Logic_err::msg_t("error submitting triple")
                  << Logic_err::str1_t(to_string(t, ts))
                  << Logic_err::str2_t(ts[t.doc_].path)
                  << Logic_err::nested_t(boost::current_exception())
         );
      }
      if( a ) try{
         ++n;
         kernel.isKBConsistent();
      }catch(...) {
         BOOST_THROW_EXCEPTION(
                  Logic_err()
                  << Logic_err::msg_t("reasoning error after submitting triple")
                  << Logic_err::str1_t(to_string(t, ts))
                  << Logic_err::str2_t(ts[t.doc_].path)
                  << Logic_err::int1_t(n)
                  << Logic_err::nested_t(boost::current_exception())
         );
      }
   }
   return n;
}

}//namespace owlcpp
