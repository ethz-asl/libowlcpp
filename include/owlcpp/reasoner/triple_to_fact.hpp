/** @file "/owlcpp/include/owlcpp/reasoner/triple_to_fact.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_HPP_
#define TRIPLE_TO_FACT_HPP_
#include "owlcpp/reasoner/config.hpp"

class OWLCPP_REASONER_DECL ReasoningKernel;

namespace owlcpp{

class Triple_store;

/** Add triples from the store to reasoning kernel
*******************************************************************************/
OWLCPP_REASONER_DECL void add(const Triple_store&, ReasoningKernel&, const bool lax = false);


}//namespace owlcpp

#endif /* TRIPLE_TO_FACT_HPP_ */
