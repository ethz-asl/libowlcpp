/** @file "/owlcpp/binding/python/rdf.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/def.hpp"
#include "boost/python/operators.hpp"
#include "boost/python/return_internal_reference.hpp"
#include "boost/utility.hpp"

#include "python/triple_store.hpp"
#include "python/triple.hpp"
namespace op = owlcpp::py;

BOOST_PYTHON_MODULE(_rdf) {
   namespace bp = boost::python;

   bp::class_<owlcpp::Ns_id>("Ns_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owlcpp::Ns_id::operator())
      ;

   bp::class_<owlcpp::Doc_id>("Doc_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owlcpp::Doc_id::operator())
      ;

   bp::class_<owlcpp::Node_id>("Node_id", bp::init<unsigned>())
      .def(str(bp::self))
      .def(bp::self < bp::self)
      .def(bp::self > bp::self)
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def("__call__", &owlcpp::Node_id::operator())
      ;

   bp::class_<op::Triple>(
            "Triple",
            "IDs for subject, predicate, object, and document",
            bp::init<
            const owlcpp::Node_id,const owlcpp::Node_id,
            const owlcpp::Node_id,const owlcpp::Doc_id
            >())
      .def(str(bp::self))
      .def(bp::self == bp::self)
      .def(bp::self != bp::self)
      .def_readonly("subj_", &op::Triple::subj_)
      .def_readonly("pred_", &op::Triple::pred_)
      .def_readonly("obj_", &op::Triple::obj_)
      .def_readonly("doc_", &op::Triple::doc_)
      ;

   bp::class_<owlcpp::Ns_iri>(
            "Ns_iri", "namespace IRI",
            bp::init<std::string>()
   )
      .def(str(bp::self))
      .def("str", &owlcpp::Ns_iri::str, bp::return_internal_reference<>())
   ;

   bp::class_<op::Triple_store, boost::noncopyable>(
            "Triple_store",
            "Store namespace IRIs, RDF nodes, ontology descriptions, and triples",
            bp::init<>()
   )
      .def(
               "map_ns",
//               "map of namespace IRIs"
               &op::Triple_store::map_ns,
               bp::return_internal_reference<>()
      )
//      .def("map_node", &owlcpp::Triple_store::map_node)
//      .def("map_doc", &owlcpp::Triple_store::map_doc)
      ;
}
