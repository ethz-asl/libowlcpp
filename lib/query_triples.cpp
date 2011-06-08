/** @file "/owl_cpp/lib/query_triples.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#include "owl_cpp/query_triples.hpp"
#include "boost/foreach.hpp"
#include "owl_cpp/query_nodes.hpp"
#include "node_type.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp{ namespace{
/**
Recursively accumulate members of the collection
*******************************************************************************/
void rdf_collection(
      const Node_id nid,
      const Triple_store& store,
      std::vector<Node_id>& rdfc
) {
   //end of collection found
   if( nid == ot::T_rdf_nil::id() ) return;

   rdfc.push_back(
         find_first_triple(nid, ot::T_rdf_first::id(), blank(), store).object()
   );

   rdf_collection(
         find_first_triple(nid, ot::T_rdf_rest::id(), blank(), store).object(),
         store, rdfc
   );
}

}//namespace anonymous

/*
*******************************************************************************/
std::vector<Node_id> rdf_collection(const Node_id nid, const Triple_store& store) {
   std::vector<Node_id> rdfc;
   rdf_collection(nid, store, rdfc);
   return rdfc;
}

/*
*******************************************************************************/
bool have_ontology_id(const std::string& iri, const Triple_store& store) {
   const b::tuple<std::string,std::string> t = Triple_store::split(iri);
   if( ! store.find(t.get<0>()) ) return false;
   const Ns_id ns_id = store[t.get<0>()];
   Triple_store::node_search_range_t r =
         store.find( node_generic_t(ns_id, t.get<1>()) );
   if( ! r ) return false;
   const Node_id nid = r.front().second;

   if(
         find_triples(
               nid,
               ot::T_rdf_type::id(),
               ot::T_owl_Ontology::id(),
               store
         )
   ) return true;

   if(
         find_triples(
               blank(),
               ot::T_owl_versionIRI::id(),
               nid,
               store
         )
   ) return true;
   return false;
}

/*
*******************************************************************************/
std::size_t num_ontologies(const Triple_store& store) {
   return distance(
         find_triples(
               blank(),
               ot::T_rdf_type::id(),
               ot::T_owl_Ontology::id(),
               store
         )
   );
}

/*
*******************************************************************************/
}//namespace owl_cpp
