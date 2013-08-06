/** @file "/owlcpp/binding/python/rdf_triple_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/ns_iri.hpp"
#include "owlcpp/rdf/query_triple_any.hpp"
#include "python/node_store_api.hpp"
using owlcpp::Triple_store;
using owlcpp::Node_id;

void export_triple_store() {
   bp::class_<Triple_store> ts(
            "Triple_store",
            "Store namespace IRIs, RDF nodes, ontology descriptions, and triples"
   );
   owlcpp::py::export_node_store_api(ts)
   .def(
            "insert_literal",
            static_cast<
               Node_id (Triple_store::*)
               (std::string const&, std::string const&, std::string const&)
            >(&Triple_store::insert_literal),
            "TODO: doc string"
   )

   .def(
            "insert_blank",
            &Triple_store::insert_blank,
            "TODO: doc string"
   )

   .def(
            "map_triple",
            &Triple_store::map_triple,
            bp::return_internal_reference<>(),
            "map of RDF triples"
   )

   .def(
            "insert",
            static_cast<void (Triple_store::*)(owlcpp::Triple const&)>
               (&Triple_store::insert),
            "insert new triple"
   )

   .def(
            "find_triple",
            &owlcpp::find_triple,
            (
                     bp::arg("subj")=(Node_id const*)0,
                     bp::arg("pred")=(Node_id const*)0,
                     bp::arg("obj")=(Node_id const*)0,
                     bp::arg("doc")=(Doc_id const*)0
            ),
            "find triples matching any combination of subject, predicate, "
            "object, or document"
   )
   ;

bp::class_<owlcpp::triple_any_range>("Triple_range", bp::no_init)
   .def("__iter__", bp::iterator<owlcpp::triple_any_range>())
   ;
}


