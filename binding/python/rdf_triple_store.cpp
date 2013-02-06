/** @file "/owlcpp/binding/python/rdf_triple_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_triple_any.hpp"

namespace{
owlcpp::Doc_id insert_doc(
         owlcpp::Triple_store& ts,
         std::string const& iri,
         std::string const& path,
         std::string const& vers
         ) {
   return ts.insert_doc(iri, path, vers).first;
}

}//namespace anonymous

void export_triple_store() {

bp::class_<owlcpp::Triple_store>(
         "Triple_store",
         "Store namespace IRIs, RDF nodes, ontology descriptions, and triples",
         bp::init<>()
)
   .def(
            "map_ns",
            &owlcpp::Triple_store::map_ns,
            bp::return_internal_reference<>(),
            "map of namespace IRIs"
   )

   .def(
            "map_node",
            &owlcpp::Triple_store::map_node,
            bp::return_internal_reference<>(),
            "map of RDF nodes"
   )

   .def(
            "map_doc",
            &owlcpp::Triple_store::map_doc,
            bp::return_internal_reference<>(),
            "map of ontology documents"
   )

   .def(
            "__getitem__",
            static_cast<
               owlcpp::Ns_iri const&
               (owlcpp::Triple_store::*)(owlcpp::Ns_id const) const
            >(&owlcpp::Triple_store::at),
            bp::return_internal_reference<>(),
            "TODO: doc string"
   )

   .def(
            "__getitem__",
            static_cast<
               owlcpp::Node const&
               (owlcpp::Triple_store::*)(owlcpp::Node_id const) const
            >(&owlcpp::Triple_store::at),
            bp::return_internal_reference<>(),
            "TODO: doc string"
   )

   .def(
            "__getitem__",
            static_cast<
               owlcpp::Doc_meta const&
               (owlcpp::Triple_store::*)(owlcpp::Doc_id const) const
            >(&owlcpp::Triple_store::at),
            bp::return_internal_reference<>(),
            "TODO: doc string"
   )

   .def(
            "insert",
            static_cast<
               owlcpp::Ns_id (owlcpp::Triple_store::*)(owlcpp::Ns_iri const&)
            >(&owlcpp::Triple_store::insert),
            "TODO: doc string"
   )

   .def(
            "insert_node_iri",
            static_cast<
               owlcpp::Node_id (owlcpp::Triple_store::*)(std::string const&)
            >(&owlcpp::Triple_store::insert_node_iri),
            "TODO: doc string"
   )

   .def(
            "insert_literal",
            static_cast<
               owlcpp::Node_id (owlcpp::Triple_store::*)
               (std::string const&, std::string const&, std::string const&)
            >(&owlcpp::Triple_store::insert_literal),
            "TODO: doc string"
   )

   .def(
            "insert_blank",
            &owlcpp::Triple_store::insert_blank,
            "TODO: doc string"
   )

   .def(
            "insert_doc",
            insert_doc,
            (bp::arg("iri"), bp::arg("path")="", bp::arg("vers")=""),
            "TODO: doc string"
   )

   .def(
            "insert_triple",
            &owlcpp::Triple_store::insert_triple,
            "TODO: doc string"
   )

   .def(
            "find_triple",
            &owlcpp::find_triple,
            (
                     bp::arg("subj")=(owlcpp::Node_id const*)0,
                     bp::arg("pred")=(owlcpp::Node_id const*)0,
                     bp::arg("obj")=(owlcpp::Node_id const*)0,
                     bp::arg("doc")=(owlcpp::Doc_id const*)0
            ),
            "TODO: doc string"
   )
   ;

bp::class_<owlcpp::triple_any_range_t>("Triple_range", bp::no_init)
   .def("__iter__", bp::iterator<owlcpp::triple_any_range_t>())
   ;
}


