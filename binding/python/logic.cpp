/** @file "/owlcpp/binding/python/logic.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

#include "factpp/Kernel.hpp"
#include "owlcpp/logic/triple_to_fact.hpp"
#include "owlcpp/rdf/triple_store.hpp"

void export_factpp_kernel();
void export_factpp_expression_manager();
void export_factpp_expressions();

BOOST_PYTHON_MODULE(_logic) {
   export_factpp_expressions();
   export_factpp_expression_manager();
   export_factpp_kernel();

   bp::def(
            "submit",
            static_cast<
            std::size_t (*)
            (owlcpp::Triple_store const&, ReasoningKernel&, const bool, const bool)
            >(&owlcpp::submit),
            (bp::arg("store"), bp::arg("kernel"),
                     bp::arg("strict")=true, bp::arg("diagnose")=false),
            "Convert RDF triples from the triple store to axioms and submit "
            "them to reasoning kernel"
   );
}
