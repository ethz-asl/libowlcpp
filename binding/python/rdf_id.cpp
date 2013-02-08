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
using owlcpp::Node_id;
using owlcpp::Doc_id;
using owlcpp::Ns_id;

void export_ids() {
   bp::class_<Ns_id>("Ns_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &Ns_id::operator())
      ;

   bp::class_<Doc_id>("Doc_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &Doc_id::operator())
      ;

   bp::class_<Node_id>("Node_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &Node_id::operator())
      ;
}
