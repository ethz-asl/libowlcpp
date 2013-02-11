#!/usr/bin/env python
'''
owlcpp/binding/python/test/test_logic_1.py is part of owlcpp project
Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
Copyright Mikhail K Levin 2011-3
'''
import os, unittest
from owlcpp import *

class Test(unittest.TestCase):
    def setUp(self):
        unittest.TestCase.setUp(self)
        self.dir = 'sample_data'
        self.path1 = os.path.abspath(os.path.join(self.dir, 'og_01.owl'))
        self.path2 = os.path.abspath(os.path.join(self.dir, 'one_eq_two_01.owl'))

    def test_1(self):
        ts = Triple_store();
        load_file(self.path1, ts)
        k = ReasoningKernel();
        submit(ts, k)
        self.assertTrue(k.isKBConsistent())

    def test_1(self):
        ts = Triple_store();
        load_file(self.path2, ts)
        k = ReasoningKernel();
        submit(ts, k)
        self.assertFalse(k.isKBConsistent())
        

if __name__ == '__main__': unittest.main()
