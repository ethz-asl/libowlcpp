/** @file "/owl_cpp/include/owl_cpp/triple_to_fact.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_HPP_
#define TRIPLE_TO_FACT_HPP_
#include <memory>
class ReasoningKernel;

namespace owl_cpp{

class Triple_store;

/** Add triples from the store to reasoning kernel
*******************************************************************************/
void add(const Triple_store&, ReasoningKernel&, const bool lax = false);


}//namespace owl_cpp

#endif /* TRIPLE_TO_FACT_HPP_ */
