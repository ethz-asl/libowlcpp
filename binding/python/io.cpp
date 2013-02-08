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
using owlcpp::Triple_store;
using owlcpp::Catalog;

void export_catalog();

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
   owlcpp::load_file(file, ts);
}

void load_file_2(
         boost::filesystem::path const& file,
         Triple_store& ts,
         Catalog const& cat
) {
   owlcpp::load_file(file, ts, cat);
}

}//namespace anonymous

BOOST_PYTHON_MODULE(_io) {
   bp::register_exception_translator<owlcpp::base_exception>(&translator);
   bp::implicitly_convertible<std::string,boost::filesystem::path>();
   export_catalog();

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
         &load_file_1,
         (bp::arg("file"), bp::arg("store")),
         "load ontology document ignoring imports"
   );

   bp::def(
         "load_file",
         &load_file_2,
         (bp::arg("file"), bp::arg("store"), bp::arg("catalog")),
         "load ontology document including its imports"
   );

   bp::def(
         "load_iri",
         static_cast<
            void (*) (
                     std::string const&,
                     Triple_store&,
                     Catalog const&
                  )
         >(&owlcpp::load_iri)
   );
}
