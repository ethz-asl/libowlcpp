/** @file "/owlcpp/include/owlcpp/logic/triple_to_fact.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_HPP_
#define TRIPLE_TO_FACT_HPP_
//#include "owlcpp/logic/config.hpp"
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "factpp/config.hpp"

class /*FACTPP_KERNEL_DECL*/ ReasoningKernel;

namespace owlcpp{

/** Copy triples from store to reasoning kernel
*******************************************************************************/
template<class Range> inline void submit_triples(
         Range r,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool lax
) {
   detail::Triple_to_fact_adaptor ttfa(ts, kernel, lax);
   ttfa.submit(r);
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
