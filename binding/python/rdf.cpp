/** @file "/owlcpp/binding/python/rdf.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/def.hpp"
#include "boost/python/operators.hpp"
#include "boost/utility.hpp"

#include "owlcpp/rdf/triple_store.hpp"

BOOST_PYTHON_MODULE(rdf) {
   namespace bp = boost::python;

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

/*
   bp::class_<owlcpp::Triple>("Triple")
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("subject", &owlcpp::Triple::get<0>())
      .def("predicate", &owlcpp::Triple::get<1>())
      .def("object", &owlcpp::Triple::get<2>())
      ;
*/

   bp::class_<owlcpp::Triple_store, boost::noncopyable>("Triple_store")
//      .def("path", &owlcpp::Triple_store::path)
      ;
}
