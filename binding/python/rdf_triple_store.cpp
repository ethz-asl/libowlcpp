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
struct Triple_store : public owlcpp::Triple_store {
   owlcpp::Ns_id insert_iri(owlcpp::Ns_iri const& iri) {return insert(iri);}
};
*******************************************************************************/

//owlcpp::Ns_id (owlcpp::Triple_store::*insert_iri)(owlcpp::Ns_iri const&) =
//         owlcpp::Triple_store::insert;
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
//      .def("map_node", &owlcpp::Triple_store::map_node)
//      .def("map_doc", &owlcpp::Triple_store::map_doc)
   .def(
            "insert",
            static_cast<
            owlcpp::Ns_id (owlcpp::Triple_store::*)(owlcpp::Ns_iri const&)
            >(&owlcpp::Triple_store::insert)
   )
   ;
}


