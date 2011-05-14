/** @file "/owl_cpp/include/owl_cpp/triple_to_fact.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
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
