/** @file "/owlcpp/lib/test/sample_triples.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef SAMPLE_TRIPLES_HPP_
#define SAMPLE_TRIPLES_HPP_
#include "owlcpp/rdf/triple_store.hpp"

namespace owlcpp{ namespace test{

const std::string ns1 = "http://ns1";
const std::string ns2 = "http://ns2";
const std::string ns3 = "http://ns3";

const std::string doc1 = "http://doc1";
const std::string doc2 = "http://doc2";
const std::string doc3 = "http://doc3";

const std::string path1 = "path1";
const std::string path2 = "path2";
const std::string path3 = "path3";

const std::string iri11 = ns1 + "#" + "node1";
const std::string iri12 = ns1 + "#" + "node2";
const std::string iri13 = ns1 + "#" + "node3";
const std::string iri14 = ns1 + "#" + "node4";

const std::string iri21 = ns2 + "#" + "node1";
const std::string iri22 = ns2 + "#" + "node2";
const std::string iri23 = ns2 + "#" + "node3";
const std::string iri24 = ns2 + "#" + "node4";

/**@brief 
*******************************************************************************/
Triple_store& sample_triples_01(Triple_store& ts) {
   const Node_id nid1 = ts.insert_node_iri(iri11);
   const Node_id nid2 = ts.insert_node_iri(iri12);
   const Node_id nid3 = ts.insert_node_iri(iri13);
   const Node_id nid4 = ts.insert_node_iri(iri14);
   const Doc_id did1 = ts.insert_doc(doc1, path1, "").first;
   const Node_id nid5 = ts.insert_blank(1, did1);
   const Node_id nid6 = ts.insert_blank(2, did1);
   const Node_id nid7 = ts.insert_blank(3, did1);
   const Node_id nid8 = ts.insert_blank(4, did1);
   const Node_id nid9 = ts.insert_blank(5, did1);

   ts.insert_triple(nid1, nid2, nid3, did1);
   ts.insert_triple(nid4, nid2, nid3, did1);
   ts.insert_triple(nid5, nid2, nid3, did1);
   ts.insert_triple(nid6, nid2, nid3, did1);
   ts.insert_triple(nid7, nid2, nid3, did1);
   ts.insert_triple(nid8, nid2, nid3, did1);
   ts.insert_triple(nid9, nid2, nid3, did1);

   return ts;
}

Triple_store sample_triples_01() {
   Triple_store ts;
   return sample_triples_01(ts);
}

/**@brief
*******************************************************************************/
Triple_store& sample_triples_02(Triple_store& ts) {
   const Doc_id did1 = ts.insert_doc(doc2, path2, doc3).first;
   const Node_id nid5 = ts.insert_blank(1, did1);
   const Node_id nid6 = ts.insert_blank(2, did1);
   const Node_id nid7 = ts.insert_blank(3, did1);
   const Node_id nid8 = ts.insert_blank(4, did1);
   const Node_id nid9 = ts.insert_blank(5, did1);
   const Node_id nid1 = ts.insert_node_iri(iri21);
   const Node_id nid2 = ts.insert_node_iri(iri12);
   const Node_id nid3 = ts.insert_node_iri(iri23);
   const Node_id nid4 = ts.insert_node_iri(iri14);

   ts.insert_triple(nid1, nid2, nid3, did1);
   ts.insert_triple(nid4, nid2, nid3, did1);
   ts.insert_triple(nid5, nid2, nid3, did1);
   ts.insert_triple(nid6, nid2, nid3, did1);
   ts.insert_triple(nid7, nid2, nid3, did1);
   ts.insert_triple(nid8, nid2, nid3, did1);
   ts.insert_triple(nid9, nid2, nid3, did1);

   return ts;
}

Triple_store sample_triples_02() {
   Triple_store ts;
   return sample_triples_02(ts);
}

}//namespace test
}//namespace owlcpp
#endif /* SAMPLE_TRIPLES_HPP_ */
