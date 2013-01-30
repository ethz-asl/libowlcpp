/** @file "/owlcpp/binding/python/io.cpp" 
part of owlcpp project.
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
#include "boost/python/implicit.hpp"
namespace bp = boost::python;
#include "boost/tuple/tuple.hpp"

#include "owlcpp/exception.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/read_ontology_iri.hpp"

namespace{
template<class T1, class T2>
struct Pair_to_tuple {
  static PyObject* convert(const std::pair<T1, T2>& pair) {
    return bp::incref(bp::make_tuple(pair.first, pair.second).ptr());
  }
};

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

void translator(owlcpp::base_exception const& e) {
   PyErr_SetString(
         PyExc_RuntimeError, boost::diagnostic_information(e).c_str()
   );
}

}//namespace anonymous

BOOST_PYTHON_MODULE(io) {
   bp::register_exception_translator<owlcpp::base_exception>(&translator);
   bp::class_<owlcpp::Catalog>("Catalog")
//      .def("insert_doc", &owlcpp::Catalog::insert_doc)
//      .def(
//            "find_location",
//            &owlcpp::Catalog::find_location,
//            bp::return_value_policy<bp::return_by_value>()
//            )
      ;
   bp::implicitly_convertible<std::string,boost::filesystem::path>();
//   Tuple2tuple_converter<std::string, std::string>();
   bp::def("ontology_id",
         static_cast<
            std::pair<std::string,std::string> (*)
            (boost::filesystem::path const&, const std::size_t)
         >(&owlcpp::read_ontology_iri)
   );

   bp::def(
         "load_file",
         static_cast<
            void (*) (boost::filesystem::path const&, owlcpp::Triple_store&)
         >(&owlcpp::load_file)
   );

   bp::def(
         "load_file",
         static_cast<
            void (*) (
                     boost::filesystem::path const&,
                     owlcpp::Triple_store&,
                     owlcpp::Catalog const&
                  )
         >(&owlcpp::load_file)
   );
}
