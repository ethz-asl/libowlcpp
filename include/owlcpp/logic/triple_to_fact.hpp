/** @file "/owlcpp/include/owlcpp/logic/triple_to_fact.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_HPP_
#define TRIPLE_TO_FACT_HPP_
#include "owlcpp/logic/detail/triple_to_fact_adaptor.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "factpp/config.hpp"

class /*FACTPP_KERNEL_DECL*/ ReasoningKernel;

namespace owlcpp{

/** Convert a range of RDF triples to axioms and submit them to reasoning kernel
@param r iterator range of triples
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced
@return number of axioms generated
*******************************************************************************/
template<class Range> inline std::size_t submit(
         Range r,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true
) {
   logic::factpp::Adaptor_triple ttfa(ts, kernel, strict);
   return ttfa.submit(r);
}

/** Convert all stored RDF triples to axioms and submit them to reasoning kernel
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced
@return number of axioms generated
*******************************************************************************/
inline std::size_t submit(
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true
) {
   return submit(ts.map_triple(), ts, kernel, strict);
}

/**Example of submitting triples to reasoner
@example validate.cpp
*/

}//namespace owlcpp

#endif /* TRIPLE_TO_FACT_HPP_ */
