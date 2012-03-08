/** @file "/owlcpp/include/owlcpp/rdf/query_triples.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef QUERY_TRIPLES_HPP_
#define QUERY_TRIPLES_HPP_
#include "boost/range.hpp"
#include "owlcpp/rdf/rdf_list_iterator.hpp"

namespace owlcpp{

/**
@param nid node ID of the subject of the first triple of RDF list
@param tm map of triples
@return range of nodes corresponding to RDF list starting from @b nid
@details Example of a list:
@code
_:c1 rdf:first <ex:aaa>
_:c1 rdf:rest _:c2
_:c2 rdf:first <ex:bbb>
_:c2 rdf:rest rdf:nil
@endcode
*******************************************************************************/
boost::iterator_range<Rdf_list_iter_m> rdf_list(const Node_id nid, Triple_map const& tm) {
   return boost::make_iterator_range(Rdf_list_iter_m(nid, tm), Rdf_list_iter_m());
}

/**
@param nid node ID of the subject of the first triple of RDF list
@param ts triple store
@return range of nodes corresponding to RDF list starting from @b nid
@details Example of a list:
@code
_:c1 rdf:first <ex:aaa>
_:c1 rdf:rest _:c2
_:c2 rdf:first <ex:bbb>
_:c2 rdf:rest rdf:nil
@endcode
*******************************************************************************/
boost::iterator_range<Rdf_list_iter_s> rdf_list(const Node_id nid, Triple_store const& ts) {
   return boost::make_iterator_range(Rdf_list_iter_s(nid, ts), Rdf_list_iter_s());
}


}//namespace owlcpp
#endif /* QUERY_TRIPLES_HPP_ */
