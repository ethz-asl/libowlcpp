#!/usr/bin/env python

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

def run(args = None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest
    return doctest.testmod(verbose=True)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
