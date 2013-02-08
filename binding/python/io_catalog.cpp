/** @file "/owlcpp/binding/python/io_catalog.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;
#include <limits>

#include "owlcpp/io/catalog.hpp"
#include "python/node_store_api.hpp"
using owlcpp::Catalog;
using owlcpp::Triple_store;
using owlcpp::Doc_id;

void export_catalog() {

   bp::class_<Catalog> cat("Catalog");

   owlcpp::py::export_node_store_api(cat)
   .def("ontology_iri_str", &Catalog::ontology_iri_str)
   .def("version_iri_str", &Catalog::version_iri_str)

//      .def(
//            "find_location",
//            &owlcpp::Catalog::find_location,
//            bp::return_value_policy<bp::return_by_value>()
//            )
      ;

   bp::def(
            "add",
            &owlcpp::add,
            (
                     bp::arg("catalog"),
                     bp::arg("path"),
                     bp::arg("recurse")=false,
                     bp::arg("depth")=std::numeric_limits<std::size_t>::max()
            )
   );
}
