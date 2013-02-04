/** @file "/owlcpp/binding/python/rdf_triple_store.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;

#include "owlcpp/rdf/triple_store.hpp"

namespace{

/**@brief
*******************************************************************************/
struct Triple_store : public owlcpp::Triple_store {
   owlcpp::Ns_id insert_iri(owlcpp::Ns_iri const& iri) {return insert(iri);}
};

}//namespace anonymous

void export_triple_store() {


bp::class_<owlcpp::Triple_store, boost::noncopyable>(
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
            bp::return_internal_reference<>()
   )

   .def(
            "__getitem__",
            static_cast<
               owlcpp::Node const&
               (owlcpp::Triple_store::*)(owlcpp::Node_id const) const
            >(&owlcpp::Triple_store::at),
            bp::return_internal_reference<>()
   )

   .def(
            "__getitem__",
            static_cast<
               owlcpp::Doc_meta const&
               (owlcpp::Triple_store::*)(owlcpp::Doc_id const) const
            >(&owlcpp::Triple_store::at),
            bp::return_internal_reference<>()
   )

   .def(
            "insert",
            static_cast<
               owlcpp::Ns_id (owlcpp::Triple_store::*)(owlcpp::Ns_iri const&)
            >(&owlcpp::Triple_store::insert)
   )

   .def(
            "insert_node_iri",
            static_cast<
               owlcpp::Node_id (owlcpp::Triple_store::*)(std::string const&)
            >(&owlcpp::Triple_store::insert_node_iri)
   )

   .def(
            "insert_literal",
            static_cast<
               owlcpp::Node_id (owlcpp::Triple_store::*)
               (std::string const&, std::string const&, std::string const&)
            >(&owlcpp::Triple_store::insert_literal)
   )

   .def(
            "insert_blank",
            static_cast<
               owlcpp::Node_id (owlcpp::Triple_store::*)
               (const unsigned, const owlcpp::Doc_id)
            >(&owlcpp::Triple_store::insert_blank)
   )

   .def(
            "insert_doc",
            static_cast<
               std::pair<owlcpp::Doc_id,bool> (owlcpp::Triple_store::*)
               (std::string const&, std::string const&, std::string const&)
            >(&owlcpp::Triple_store::insert_doc)
   )
   ;
}


