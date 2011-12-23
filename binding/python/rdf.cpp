/** @file "/owl_cpp/binding/python/rdf.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/def.hpp"
#include "boost/utility.hpp"

#include "owl_cpp/rdf/triple_store.hpp"

BOOST_PYTHON_MODULE(rdf) {
   namespace owl = owl_cpp;
   namespace bp = boost::python;
//   class_<Node_id>("Node_id", init<unsigned>())

   bp::class_<owl::Triple_store, boost::noncopyable>("Triple_store")
      .def("n_uris", &owl::Triple_store::n_uris)
      .def("n_nodes", &owl::Triple_store::n_nodes)
      .def("n_triples", &owl::Triple_store::n_triples)
      ;
}
