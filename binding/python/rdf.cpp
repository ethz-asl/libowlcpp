/** @file "/owlcpp/binding/python/rdf.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

void export_ids();
void export_maps();
void export_triple_store();
void export_objects();

BOOST_PYTHON_MODULE(_rdf) {
   bp::object package = bp::scope();
   package.attr("__path__") = "_rdf";
   export_ids();
   export_maps();
   export_triple_store();
   export_objects();
}
