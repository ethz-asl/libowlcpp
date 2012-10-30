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
   namespace owl = owlcpp;
   namespace bp = boost::python;

   bp::class_<owl::Node_id>("Node_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owl::Node_id::operator())
      ;

/*
   bp::class_<owl::Triple>("Triple")
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("subject", &owl::Triple::get<0>())
      .def("predicate", &owl::Triple::get<1>())
      .def("object", &owl::Triple::get<2>())
      ;
*/

   bp::class_<owl::Triple_store, boost::noncopyable>("Triple_store")
      .def("n_uris", &owl::Triple_store::n_uris)
      .def("n_nodes", &owl::Triple_store::n_nodes)
      .def("n_triples", &owl::Triple_store::n_triples)
      ;
}
