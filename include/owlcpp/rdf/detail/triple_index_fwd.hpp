/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_fwd.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_FWD_HPP_
#define TRIPLE_INDEX_FWD_HPP_

namespace owlcpp{ namespace map_triple_detail{

/**@brief Store RDF triples in a searchable fashion
@tparam Map a map between Tag0 elements and <Tag1,Tag2,Tag3> triple fragments
@tparam Tag0 numerical tag indicating which triple element is indexed first
@tparam Tag1 numerical tag indicating which triple element is indexed second
@tparam Tag2 numerical tag indicating which triple element is indexed third
@tparam Tag3 numerical tag indicating which triple element is indexed fourth
*******************************************************************************/
template<
   template<class,class,class,class> class Map,
   class Tag0,
   class Tag1,
   class Tag2,
   class Tag3
> class Triple_index;

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_FWD_HPP_ */
