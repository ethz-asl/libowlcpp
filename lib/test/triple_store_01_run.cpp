/** @file "/owlcpp/lib/test/triple_store_01_run.cpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
//#include "pch.hpp"
#define BOOST_TEST_MODULE triple_store_01_run
#include "boost/test/unit_test.hpp"
#include <iostream>

#include "sample_data.hpp"

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/term_tags.hpp"
#include "owlcpp/print.hpp"
namespace ot = owlcpp::terms;
#include "node_type.hpp"
#include "triple_store_checks.hpp"
#include "owlcpp/rdf/query_nodes.hpp"
#include "owlcpp/rdf/query_triples.hpp"

namespace owlcpp{ namespace test{

/**
*******************************************************************************/
struct Visitor_get_name
: public L::BaseVisitor,
  public L::Visitor<Node_std<ot::T_owl_Class>, void, true>,
  public L::Visitor<Node_std<ot::T_rdf_Property>, void, true>,
  public L::Visitor<Node_base, void, true>
{

   void Visit(const Node_std<ot::T_owl_Class>& term) {
      std::cout
      << "Node_std<ot::T_owl_Class> "
      << term.value_str() << std::endl;
      name = term.value_str();
   }

   void Visit(const Node_std<ot::T_rdf_Property>& term) {
      std::cout
      << "Node_std<ot::T_rdf_Property> "
      << term.value_str() << std::endl;
      name = term.value_str();
   }

   void Visit(const Node_base& term) {
      std::cout
      << "Node_base "
      << term.value_str() << std::endl;
      name = term.value_str();
   }

   std::string name;
};

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( triple_store_01_visitor_test ) {
   Triple_store store;
   BOOST_CHECK( check_and_print(store, std::cout) );
   Visitor_get_name vgn;
   const Node_base& t1 = store[Node_id(ot::T_owl_Class::index)];
   t1.Accept(vgn);
   BOOST_CHECK_EQUAL(ot::T_owl_Class::name(), vgn.name);

   const Node_base& t2 = store[ Node_id(ot::T_rdf_Property::index) ];
   t2.Accept(vgn);
   BOOST_CHECK_EQUAL(ot::T_rdf_Property::name(), vgn.name);
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( triple_store_01_run_case01 ) {
   Triple_store store;
   BOOST_CHECK_EQUAL(store[Ns_id(ot::N_owl::index)], ot::N_owl::iri());
   Ns_id id1 = store.insert_namespace(ot::N_xsd::iri());
   BOOST_CHECK(id1() == ot::N_xsd::index);

   const Node_base& ti = store[ ot::T_owl_Class::id() ];

   std::cout << '\n'
   << short_name( ot::T_owl_Class::id(), store )
   << std::endl;

   Visitor_get_name vgn;
   ti.Accept(vgn);
   BOOST_CHECK_EQUAL(ot::T_owl_Class::name(), vgn.name);

   const unsigned i = ot::T_owl_Class::ns_type::index;
   const unsigned j = ot::T_owl_Class::ns_type::id()();
   BOOST_CHECK_EQUAL(i, j);
   BOOST_CHECK_EQUAL(i, ti.ns_id()());
   BOOST_CHECK_EQUAL(ot::T_owl_Class::name(), ti.value_str());

   Node_id id2 = store.insert_node(Node_std<ot::T_rdfs_subClassOf>());
   Node_id id3 = store.insert_node(Node_std<ot::T_rdfs_subClassOf>());
   BOOST_CHECK( id2() == ot::T_rdfs_subClassOf::index );
   BOOST_CHECK( check_and_print(store, std::cout) );
}

/** Generic resource terms containing standard name spaces and keywords
are equal to the corresponding standard classes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( triple_store_01_run_case02 ) {
   Triple_store store;
   node_generic_t tr(ot::T_rdfs_subClassOf::ns_type::id(), ot::T_rdfs_subClassOf::name());
   const Node_base& ti = store[ ot::T_rdfs_subClassOf::id() ];
   BOOST_CHECK(tr == ti);
   BOOST_CHECK( store.insert_node(tr) == ot::T_rdfs_subClassOf::id() );
}

/**
*******************************************************************************/
BOOST_AUTO_TEST_CASE( triple_store_01_run_case03 ) {
   Triple_store store;
   Node_id tid1 = insert_node("http://www.blah.com/blah/blah#BlahBlah", store);
   const Node_base& t1 = store[tid1];
   BOOST_CHECK_EQUAL(t1.value_str(), "BlahBlah");
   BOOST_CHECK_EQUAL(store[t1.ns_id()], "http://www.blah.com/blah/blah");

   Node_id tid2 = insert_node("http://www.blah.com/blah/blah#", store);
   const Node_base& t2 = store[tid2];
   BOOST_CHECK_EQUAL(t2.value_str(), "");
   BOOST_CHECK_EQUAL(store[t2.ns_id()], "http://www.blah.com/blah/blah");

   const std::string s("http://www.blah.com/blah/blah");
   BOOST_CHECK_EQUAL(s.find('#'), std::string::npos);
   Node_id tid3 = insert_node(s, store);
   const Node_base& t3 = store[tid3];
   BOOST_CHECK_EQUAL(t3.value_str(), "blah");
   BOOST_CHECK_EQUAL(store[t3.ns_id()], "http://www.blah.com/blah/");
}

/** Keep track of loaded ontologies
*******************************************************************************/
BOOST_AUTO_TEST_CASE( ontology_iris_case04 ) {
   Triple_store store;
   const std::string iri1("http://www.blah.com/blah/blah#BlahBlah");
   const std::string iri2("http://www.blah.com/blah/blahblah/BlahBlah");
   BOOST_CHECK( ! have_ontology_id(iri1, store) );
   store.insert(
         insert_node(iri1, store),
         ot::T_rdf_type::id(),
         ot::T_owl_Ontology::id()
   );
   BOOST_CHECK( have_ontology_id(iri1, store) );
   BOOST_CHECK( ! have_ontology_id(iri2, store) );
   store.insert(
         insert_node(iri2, store),
         ot::T_rdf_type::id(),
         ot::T_owl_Ontology::id()
   );
   BOOST_CHECK( have_ontology_id(iri2, store) );
}

}//namespace test
}//namespace owlcpp
