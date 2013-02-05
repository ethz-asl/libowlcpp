/** @file "/owlcpp/binding/python/rdf_map.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python/iterator.hpp"
namespace bp = boost::python;

#include "owlcpp/rdf/map_ns.hpp"

void export_maps() {

   bp::class_<owlcpp::Map_ns>("Map_ns", "map of namespace IRIs", bp::init<>())
      .def("__len__", &owlcpp::Map_ns::size)
      .def("empty", &owlcpp::Map_ns::empty)
      .def("__getitem__", &owlcpp::Map_ns::at, bp::return_internal_reference<>())
      .def("__iter__", bp::iterator<owlcpp::Map_ns>())
   ;

}
