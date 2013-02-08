/** @file "/owlcpp/binding/python/rdf_map.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python/iterator.hpp"
namespace bp = boost::python;

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/map_node_iri.hpp"
using owlcpp::Map_ns;
using owlcpp::Map_node;
using owlcpp::Map_node_iri;
using owlcpp::Map_doc;

void export_maps() {

   bp::class_<Map_ns>("Map_ns", "map of namespace IRIs", bp::init<>())
      .def("__len__", &Map_ns::size)
      .def("empty", &Map_ns::empty)
      .def("__getitem__", &Map_ns::at, bp::return_internal_reference<>())
      .def("__iter__", bp::iterator<Map_ns>())
   ;

   typedef owlcpp::Triple_store::map_triple_type map_triple_type;
   bp::class_<map_triple_type>("Map_triple", "map of triples", bp::init<>())
      .def("__len__", &map_triple_type::size)
      .def("empty", &map_triple_type::empty)
      .def("__iter__", bp::iterator<map_triple_type>())
   ;

   typedef Map_doc::iri_range doc_iri_range;
   bp::class_<doc_iri_range>("doc_iri_range", bp::no_init)
      .def("__iter__", bp::iterator<doc_iri_range>())
      ;

   typedef Map_doc::version_range doc_version_range;
   bp::class_<doc_version_range>("doc_version_range", bp::no_init)
      .def("__iter__", bp::iterator<doc_version_range>())
      ;

}
