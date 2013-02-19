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

    def test_1(self):
        ts = Triple_store();
        load_file(os.path.join(self.dir, 'og_01.owl'), ts)
        k = ReasoningKernel();
        submit(ts, k)
        self.assertTrue(k.isKBConsistent())

    def test_2(self):
        ts = Triple_store();
        load_file(os.path.join(self.dir, 'one_eq_two_01.owl'), ts)
        k = ReasoningKernel();
        submit(ts, k)
        self.assertFalse(k.isKBConsistent())

    def test_3(self):
        ts = Triple_store();
        load_file(os.path.join(self.dir, 'owl2-rl-rules-fp-differentFrom.owl'), ts)
        k = ReasoningKernel();
        submit(ts, k)
        em = k.getExpressionManager()
        e1 = em.Individual("http://owl2.test/rules/X1")
        e2 = em.Individual("http://owl2.test/rules/X2")
        self.assertFalse(k.isSameIndividuals(e1, e2))

    def test_4(self):
        ts = Triple_store();
        load_file(os.path.join(self.dir, 'owl2-rl-rules-fp-sameAs.owl'), ts)
        k = ReasoningKernel();
        submit(ts, k)
        em = k.getExpressionManager()
        e1 = em.Individual("http://owl2.test/rules/X1")
        e2 = em.Individual("http://owl2.test/rules/X2")
        self.assertTrue(k.isSameIndividuals(e1, e2))


if __name__ == '__main__': unittest.main()
