#!/usr/bin/env python
'''
owlcpp/binding/python/test/test_rdf_1.py is part of owlcpp project
Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
Copyright Mikhail K Levin 2011-3
'''
import unittest
from owlcpp import *

class Test(unittest.TestCase):
    def test_node_id(self):
        nid1 = Node_id(0)
        self.assertEqual(nid1(), 0)
        self.assertEqual(str(nid1), 'Node0')
        nid2 = Node_id(42)
        self.assertEqual(nid2(), 42)
        self.assertEqual(str(nid2), 'Node42')
        self.assertGreater(nid2, nid1)

    def test_triple(self):
        t1 = Triple(Node_id(1), Node_id(2), Node_id(3), Doc_id(42))
        self.assertEqual(str(t1), 'Node1,Node2,Node3,Doc42')
        t2 = Triple(Node_id(1), Node_id(2), Node_id(3), Doc_id(43))
        self.assertNotEqual(t1, t2)
        self.assertEqual(t1.subj_(), 1)
        nid1 = t1.subj_
        self.assertEqual(nid1(), 1)
        with self.assertRaises(AttributeError): t2.subj_ = Node_id(1)

    def test_ns_iri(self):
        s = 'http://example.com'
        iri1 = Ns_iri(s)
        self.assertEqual(iri1, s)
        iri2 = Ns_iri(s)
        self.assertEqual(iri1, iri2)

    def test_triple_store_1(self):
        ts = Triple_store()
        iri1 = 'http://example1.com'
        nsid1 = ts.insert(Ns_iri(iri1))
        iri2 = 'http://example2.com'
        nsid2 = ts.insert(Ns_iri(iri2))
        self.assertNotEqual('blah', ts[nsid1])
        self.assertEqual(iri1, ts.map_ns()[nsid1])
        for triple in ts.find_triple(subj=Node_id(0)):
            print triple.subj_

    def test_find_triples(self):
        ts = Triple_store()
        docid = ts.insert_doc(iri='http://example1.com', path='path1')
        iri1 = 'http://example1.com#Node1'
        nid1 = ts.insert_node_iri(iri1)
        nid2 = ts.insert_node_iri('http://example1.com#Node2')
        nid3 = ts.insert_node_iri('http://example1.com#Node3')
        nid4 = ts.insert_node_iri('http://example1.com#Node4')
        self.assertNotEqual(nid1, nid2)
        self.assertNotEqual(nid2, nid3)
        self.assertNotEqual(nid2, nid4)
        node1 = ts[nid1]
        self.assertIn(':Node1', str(node1))
        self.assertIn(':Node1', to_string_pref(node1, ts))
        self.assertEqual(to_string(node1, ts), iri1)
        self.assertEqual(to_string_full(node1, ts), iri1)
        self.assertEqual(to_string(node1, ts), to_string(nid1, ts))

        ts.insert(Triple(nid1, nid1, nid1, docid))
        ts.insert(Triple(nid2, nid1, nid1, docid))
        ts.insert(Triple(nid3, nid1, nid1, docid))
        ts.insert(Triple(nid4, nid1, nid3, docid))
        ts.insert(Triple(nid1, nid2, nid3, docid))
        ts.insert(Triple(nid1, nid3, nid4, docid))
        ts.insert(Triple(nid1, nid2, nid3, docid))
        l = list(ts.find_triple(subj=nid1))
        self.assertEqual(len(l), 3)
        l = list(ts.find_triple(pred=nid1, obj=nid3))
        self.assertEqual(len(l), 1)

    def test_standard_ns(self):
        self.assertEqual(terms.owl.iri, 'http://www.w3.org/2002/07/owl')
        self.assertEqual(terms.owl.prefix, 'owl')
        self.assertEqual(terms.owl.id(), 5)

        self.assertEqual(terms.rdf.iri, 'http://www.w3.org/1999/02/22-rdf-syntax-ns')
        self.assertEqual(terms.rdf.prefix, 'rdf')
        self.assertEqual(terms.rdf.id(), 2)

    def test_standard_terms(self):
        self.assertGreater(terms.owl_Class.id(), 10) #45 or some other number
        self.assertEqual(terms.owl_Class.fragment, 'Class')
        self.assertEqual(terms.owl_Class.ns_type.iri, 'http://www.w3.org/2002/07/owl')

if __name__ == '__main__': unittest.main()
