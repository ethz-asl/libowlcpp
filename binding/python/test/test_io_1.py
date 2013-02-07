#!/usr/bin/env python
'''
owlcpp/binding/python/test/test_io_1.py is part of owlcpp project
Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
Copyright Mikhail K Levin 2011-3
'''
import os, unittest
from owlcpp import *

class Test(unittest.TestCase):
    def setUp(self):
        unittest.TestCase.setUp(self)
        self.iri1 = 'http://purl.obolibrary.org/obo/ido/dev/test_01.owl'
        self.ver1 = 'http://purl.obolibrary.org/obo/ido/dev/test_01a.owl'
        self.dir1 = 'sample_data'
        self.path1 = os.path.abspath(os.path.join(self.dir1, 'test_01.owl'))
        
    def test_IRI(self):
        t = read_ontology_iri(self.path1)
        self.assertEqual(t[0], self.iri1)
        self.assertEqual(t[1], self.ver1)
        
    def test_catalog(self):
        cat = Catalog()
        add(cat, self.dir1)
        self.assertEqual(cat.)
        

if __name__ == '__main__': unittest.main()
