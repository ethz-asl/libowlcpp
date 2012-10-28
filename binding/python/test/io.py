#!/usr/bin/env python

r'''
>>> import os
>>> import owlcpp.io

Test ontology ID:
>>> iri = 'http://purl.obolibrary.org/obo/ido/dev/test_01.owl'
>>> ver = 'http://purl.obolibrary.org/obo/ido/dev/test_01a.owl'
>>> loc = os.path.abspath(os.path.join('sample_data', 'test_01.owl'))
>>> t = owlcpp.io.ontology_id(loc)
>>> iri == t[0]
True
>>> ver == t[1]
True

Test Catalog:
>>> cat = owlcpp.io.Catalog()
>>> dir = 'sample_data'
>>> owlcpp.io.find_ontologies(cat, dir)
>>> loc == cat.find_location(iri)
True
>>> loc == cat.find_location(ver)
True

>>> import owlcpp.rdf
>>> ts = owlcpp.rdf.Store_triple()
>>> owlcpp.io.load('sample_data/test_01.owl', ts)
>>> ts.n_triples()
15
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
