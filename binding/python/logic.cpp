/** @file "/owlcpp/binding/python/logic.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

void export_factpp();

BOOST_PYTHON_MODULE(_logic) {
   export_factpp();
}
