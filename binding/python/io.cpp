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
#include "boost/tuple/tuple.hpp"

#include "owl_cpp/rdf/triple_store.hpp"
#include "owl_cpp/io/catalog.hpp"
#include "owl_cpp/io/parse_to_triple_store.hpp"
namespace owl = owl_cpp;
namespace bp = boost::python;

namespace{
bp::tuple ontology_id(std::string const& file) {
   boost::tuple<std::string,std::string> t = owl::ontology_id(file);
   return bp::make_tuple(t.get<0>(), t.get<1>());
}

}//namespace anonymous

BOOST_PYTHON_MODULE(io) {

   bp::class_<owl::Catalog>("Catalog")
      .def("insert", &owl::Catalog::insert)
      .def(
            "find_location",
            &owl::Catalog::find_location,
            bp::return_value_policy<bp::return_by_value>()
            )
      ;

   bp::def("ontology_id", &ontology_id);
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
