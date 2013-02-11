/** @file "/owlcpp/binding/python/factpp_expression_manager.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/

#include "boost/python.hpp"
#include "boost/noncopyable.hpp"
namespace bp = boost::python;
#include "factpp/tExpressionManager.hpp"

void export_factpp_expression_manager() {
   bp::class_<TExpressionManager, boost::noncopyable>(
            "TExpressionManager", bp::init<>()
   )
      .def(
               "clear",
               &TExpressionManager::clear,
               "clear the ontology"
      )
      ;
}
