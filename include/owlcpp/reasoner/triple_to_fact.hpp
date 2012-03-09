/** @file "/owlcpp/include/owlcpp/reasoner/triple_to_fact.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_HPP_
#define TRIPLE_TO_FACT_HPP_
#include "boost/foreach.hpp"
#include "owlcpp/reasoner/config.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "factpp/config.hpp"

class FACTPP_KERNEL_DECL ReasoningKernel;

namespace owlcpp{ namespace detail{

OWLCPP_REASONER_DECL void submit_triple(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool lax
);

}//namespace detail

/** Copy triples from store to reasoning kernel
*******************************************************************************/
template<class Range> inline void submit_triples(
         Range r,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool lax
) {
   BOOST_FOREACH(Triple const& t, r) detail::submit_triple(t, ts, kernel, lax);
}

/** Copy all triples from triple store to reasoning kernel
*******************************************************************************/
inline void submit_triples(
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool lax
) {
   submit_triples(ts.triples(), ts, kernel, lax);
}


}//namespace owlcpp

#endif /* TRIPLE_TO_FACT_HPP_ */
