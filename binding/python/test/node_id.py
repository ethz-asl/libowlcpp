#!/usr/bin/env python
'''
owlcpp/binding/python/test/node_id.py is part of owlcpp project
Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
Copyright Mikhail K Levin 2011-3
'''
import unittest
import owlcpp

r'''
>>> import owlcpp
>>> nid1 = owlcpp.Node_id(0)
>>> print nid1
Node0
>>> nid1() == 0
True
>>> nid2 = owlcpp.Node_id(1)
>>> nid1 < nid2
True
>>> nid1 <= nid2
True
>>> nid1 > nid2
False
>>> nid3 = owlcpp.Node_id(1)
>>> nid3 == nid2
True
'''

class Test(unittest.TestCase):
    def test_1(self):
        nid1 = owlcpp.Node_id(0)
        self.assertEqual(nid1(), 0)
        

if __name__ == '__main__': unittest.main()
