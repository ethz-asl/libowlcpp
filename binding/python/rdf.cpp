/** @file "/owl_cpp/binding/python/rdf.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/def.hpp"
#include "boost/utility.hpp"

#include "owl_cpp/rdf/triple_store.hpp"
using namespace owl_cpp;

BOOST_PYTHON_MODULE(rdf) {
   using namespace boost::python;
//   class_<Node_id>("Node_id", init<unsigned>())

   class_<Triple_store, boost::noncopyable>("Triple_store")
      .def("n_uris", &Triple_store::n_uris)
      .def("n_nodes", &Triple_store::n_nodes)
      .def("n_triples", &Triple_store::n_triples)
      ;
}
