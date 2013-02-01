#!/usr/bin/env python

r'''
>>> import owlcpp
>>> id = owlcpp.Ns_id(0)
'''

def run(args=None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest
    return doctest.testmod(verbose=True)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
