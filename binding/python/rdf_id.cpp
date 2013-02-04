/** @file "/owlcpp/binding/python/rdf_id.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

#include "owlcpp/ns_id.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"

void export_ids() {
   bp::class_<owlcpp::Ns_id>("Ns_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owlcpp::Ns_id::operator())
      ;

   bp::class_<owlcpp::Doc_id>("Doc_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owlcpp::Doc_id::operator())
      ;

   bp::class_<owlcpp::Node_id>("Node_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owlcpp::Node_id::operator())
      ;
}
