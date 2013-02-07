/** @file "/owlcpp/binding/python/io.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;
#include "boost/tuple/tuple.hpp"

#include "owlcpp/exception.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/io/catalog.hpp"
#include "owlcpp/io/input.hpp"
#include "owlcpp/io/read_ontology_iri.hpp"

namespace{

void translator(owlcpp::base_exception const& e) {
   PyErr_SetString(
         PyExc_RuntimeError, boost::diagnostic_information(e).c_str()
   );
}

bp::tuple read_ontology_iri(
         boost::filesystem::path const& path,
         const std::size_t depth
) {
   const std::pair<std::string,std::string> p =
            owlcpp::read_ontology_iri(path, depth);
   return bp::make_tuple(p.first, p.second);
}

void load_file_1(boost::filesystem::path const& file, Triple_store& ts) {

}

}//namespace anonymous

BOOST_PYTHON_MODULE(_io) {
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
   bp::def(
            "read_ontology_iri",
            &read_ontology_iri,
            (bp::arg("path"), bp::arg("depth")= std::numeric_limits<std::size_t>::max()),
            "Find ontologyIRI and versionIRI declarations in ontology document."
            "Once ontologyIRI is found, look for *depth* number of triples"
            "to locate versionIRI."
   );

   bp::def(
         "load_file",
         &load_file_1
//         static_cast<
//            void (*) (boost::filesystem::path const&, owlcpp::Triple_store&)
//         >(&owlcpp::load_file)
   );

   bp::def(
         "load_iri",
         static_cast<
            void (*) (
                     std::string const&,
                     owlcpp::Triple_store&,
                     owlcpp::Catalog const&
                  )
         >(&owlcpp::load_iri)
   );
}
