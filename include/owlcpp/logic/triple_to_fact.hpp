/** @file "/owlcpp/include/owlcpp/logic/triple_to_fact.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_HPP_
#define TRIPLE_TO_FACT_HPP_
#include "owlcpp/logic/config.hpp"
#include "boost/foreach.hpp"
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/rdf/triple.hpp"

class ReasoningKernel;
class TDLAxiom;

namespace owlcpp{
class Triple_store;

/** Convert a range of RDF triples to axioms and submit them to reasoning kernel
@param r iterator range of triples
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced
@param diagnose if true, perform classification after each submitted axiom
@return number of axioms generated
*******************************************************************************/
template<class Range> inline std::size_t submit(
         Range r,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true,
         const bool diagnose = false
) {
   std::size_t n = 0;
   BOOST_FOREACH(Triple const& t, r) {
      if( submit(t, ts, kernel, strict, diagnose) ) ++n;
   }
   return n;
}

/** If triple produces axiom, submit axiom to reasoning kernel
@param t triple
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced
@param diagnose use
@a submit_check(Triple const&, Triple_store const&, ReasoningKernel&, const bool)
method, if true
@return axiom pointer or 0 if no axiom was generated
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLAxiom* submit(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true,
         const bool diagnose = false
);

/** If triple produces axiom, submit axiom to reasoning kernel and
perform classification.
Use this method to test whether triple/axiom produces reasoning error.
@param t triple
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced
@return number of axioms generated
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLAxiom* submit_check(
         Triple const& t,
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true
);

/** Convert RDF triples from the triple store to axioms and submit them to
reasoning kernel
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced
@param diagnose use
@a submit_check(Triple_store const&, ReasoningKernel&, const bool) method, if true
@return number of axioms generated
*******************************************************************************/
OWLCPP_LOGIC_DECL std::size_t submit(
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true,
         const bool diagnose = false
);

/** Convert all stored RDF triples to axioms and submit them to reasoning kernel
performing classification after each submitted axiom.
Use this method to find which triple/axiom produces reasoning error.
@param ts triple store
@param kernel FaCT++ reasoning kernel
@param strict if true (default), all requirements specified in
http://www.w3.org/TR/2009/REC-owl2-mapping-to-rdf-20091027/
are enforced.  Currently, strict==false results in same behavior.
@return number of axioms generated
*******************************************************************************/
OWLCPP_LOGIC_DECL std::size_t submit_check(
         Triple_store const& ts,
         ReasoningKernel& kernel,
         const bool strict = true
);

/**Example of submitting triples to reasoner
@example validate.cpp
*/

}//namespace owlcpp

#endif /* TRIPLE_TO_FACT_HPP_ */
