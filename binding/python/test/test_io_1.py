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
        self.iri1 = 'http://owl-cpp.sf.net/test/owl/test_01.owl'
        self.ver1 = 'http://owl-cpp.sf.net/test/owl/test_01a.owl'
        self.dir1 = 'sample_data'
        self.path1 = os.path.abspath(os.path.join(self.dir1, 'test_01.owl'))
        self.path2 = os.path.abspath(os.path.join(self.dir1, 'imports_test_01.owl'))

    def test_IRI(self):
        t = read_ontology_iri(self.path1)
        self.assertEqual(t[0], self.iri1)
        self.assertEqual(t[1], self.ver1)

    def test_catalog(self):
        cat = Catalog()
        add(cat, self.dir1)
        l = list(cat.find_doc_iri(self.iri1))
        self.assertEqual(len(l), 1)
        l = list(cat.find_doc_version(self.iri1))
        self.assertEqual(len(l), 0)

    def test_load_file(self):
        ts = Triple_store()
        load_file(self.path1, ts)
        self.assertEqual(len(ts.map_triple()), 13)

    def test_load_includes(self):
        ts1 = Triple_store()
        load_file(self.path2, ts1)
        self.assertEqual(len(ts1.map_triple()), 3)

        cat = Catalog()
        add(cat, self.dir1)
        ts2 = Triple_store()
        load_file(self.path2, ts2, cat)
        self.assertEqual(len(ts2.map_triple()), 17)

if __name__ == '__main__': unittest.main()
