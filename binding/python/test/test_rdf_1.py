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
        
    def test_triple_store(self):
        ts = Triple_store()

if __name__ == '__main__': unittest.main()
