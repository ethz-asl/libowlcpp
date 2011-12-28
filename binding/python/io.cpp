/** @file "/owl_cpp/binding/python/io.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#include <string>
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/def.hpp"
#include "boost/python/return_by_value.hpp"
#include "boost/python/tuple.hpp"
#include "boost/python/to_python_converter.hpp"
#include "boost/python/exception_translator.hpp"
#include "boost/tuple/tuple.hpp"

#include "owl_cpp/exception.hpp"
#include "owl_cpp/rdf/triple_store.hpp"
#include "owl_cpp/io/catalog.hpp"
#include "owl_cpp/io/parse_to_triple_store.hpp"
namespace owl = owl_cpp;
namespace bp = boost::python;

namespace{
template<class T1, class T2> struct Tuple2tuple_converter {

  static PyObject* convert(boost::tuple<T1, T2> const& t) {
     return bp::incref(
           bp::make_tuple( t.template get<0>(), t.template get<1>() ).ptr()
     );
  }

  static PyTypeObject const* get_pytype () {return &PyTuple_Type; }

  Tuple2tuple_converter() {
     bp::to_python_converter<
        boost::tuple<T1,T2>,
        Tuple2tuple_converter<T1,T2>,
        true
     >();
  }
};

void translator(owl_cpp::base_exception const& e) {
   PyErr_SetString(
         PyExc_RuntimeError, boost::diagnostic_information(e).c_str()
   );
}

}//namespace anonymous

BOOST_PYTHON_MODULE(io) {
   bp::register_exception_translator<owl_cpp::base_exception>(&translator);
   bp::class_<owl::Catalog>("Catalog")
      .def("insert", &owl::Catalog::insert)
      .def(
            "find_location",
            &owl::Catalog::find_location,
            bp::return_value_policy<bp::return_by_value>()
            )
      ;
   Tuple2tuple_converter<std::string, std::string>();
   bp::def("ontology_id", &owl::ontology_id);
   bp::def("find_ontologies", &owl::find_ontologies);

   bp::def(
         "load",
         static_cast<
            void (*) (std::string const&, owl::Triple_store&)
         >(&owl::load)
   );

   bp::def(
         "load",
         static_cast<
            void (*) (
                  std::string const&,
                  owl::Triple_store&,
                  owl::Catalog const&
                  )
         >(&owl::load)
   );
}
