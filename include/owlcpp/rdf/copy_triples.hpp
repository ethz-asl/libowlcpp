/** @file "/owlcpp/include/owlcpp/rdf/copy_triples.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef COPY_TRIPLES_HPP_
#define COPY_TRIPLES_HPP_
#include "boost/foreach.hpp"

#include "owlcpp/doc_id.hpp"

namespace owlcpp{

/**@brief copy triples from one store to another
@details IRIs, nodes, document info-s are also copied
@param ts1 source of triples
@param ts2 destination
@throw base_exception if @b ts1 contains document already present in @b ts2
*******************************************************************************/
template<class TS1, class TS2> inline void copy_triples(TS1 const& ts1, TS2& ts2) {
   typedef typename TS1::doc_map_t doc_map_t;
   BOOST_FOREACH(const Doc_id id, ts1.documents()) {

   }
}


}//namespace owlcpp
#endif /* COPY_TRIPLES_HPP_ */
