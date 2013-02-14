/** @file "/owlcpp/binding/python/rdf_objects.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
#include "boost/static_assert.hpp"
namespace bp = boost::python;

#include "owlcpp/rdf/triple.hpp"
#include "owlcpp/rdf/ns_iri.hpp"
#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/rdf/triple_store.hpp"

using owlcpp::Triple;
using owlcpp::Node_id;
using owlcpp::Doc_id;
using owlcpp::Ns_iri;
using owlcpp::Node;
using owlcpp::Triple_store;

namespace {

Triple* make_triple(
         const Node_id subj, const Node_id pred,
         const Node_id obj, const Doc_id doc
) {
   Triple* t = new Triple();
   t->subj_ = subj;
   t->pred_ = pred;
   t-> obj_ =  obj;
   t-> doc_ =  doc;
   return t;
}

std::string to_string_1(Node const& node, Triple_store const& ts) {
   return to_string(node, ts);
}

std::string to_string_2(const Node_id nid, Triple_store const& ts) {
   return to_string(nid, ts);
}

std::string to_string_full_1(Node const& node, Triple_store const& ts) {
   return to_string_full(node, ts);
}

std::string to_string_full_2(const Node_id nid, Triple_store const& ts) {
   return to_string_full(nid, ts);
}

std::string to_string_pref_1(Node const& node, Triple_store const& ts) {
   return to_string_pref(node, ts);
}

std::string to_string_pref_2(const Node_id nid, Triple_store const& ts) {
   return to_string_pref(nid, ts);
}

}//namespace anonymous

void export_objects() {

   bp::class_<Triple>(
            "Triple",
            "IDs for subject, predicate, object, and document"
   )
      .def(
               "__init__",
               bp::make_constructor(&make_triple)/*,
               (bp::arg("subj"), bp::arg("pred"), bp::arg("obj"), bp::arg("doc"))*/
   )
      .def(str(bp::self))
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def_readonly("subj_", &Triple::subj_)
      .def_readonly("pred_", &Triple::pred_)
      .def_readonly( "obj_", &Triple:: obj_)
      .def_readonly( "doc_", &Triple:: doc_)
      ;

   bp::class_<Ns_iri>("Ns_iri", "namespace IRI", bp::init<std::string>())
      .def(str(bp::self))
      .def("str", &Ns_iri::str, bp::return_internal_reference<>())
      .def(bp::self == bp::self)
      .def(bp::self == std::string())
   ;

   bp::class_<Node, boost::noncopyable>("Node", "RDF node", bp::no_init)
      .def(str(bp::self))
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      ;

   bp::def(
            "to_string",
            &to_string_1,
            (bp::arg("node"), bp::arg("store")),
            "return string representation of a node"
   );

   bp::def(
            "to_string",
            &to_string_2,
            (bp::arg("node_id"), bp::arg("store")),
            "return string representation of a node"
   );

   bp::def(
            "to_string_full",
            &to_string_full_1,
            (bp::arg("node"), bp::arg("store")),
            "return node string with complete namespace"
   );

   bp::def(
            "to_string_full",
            &to_string_full_2,
            (bp::arg("node_id"), bp::arg("store")),
            "return node string with complete namespace"
   );

   bp::def(
            "to_string_pref",
            &to_string_pref_1,
            (bp::arg("node"), bp::arg("store")),
            "return node string with namespace prefix"
   );

   bp::def(
            "to_string_pref",
            &to_string_pref_2,
            (bp::arg("node_id"), bp::arg("store")),
            "return node string with namespace prefix"
   );
}
