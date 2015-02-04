/** @file "/owlcpp/include/owlcpp/rdf/map_triple_fwd.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef MAP_TRIPLE_FWD_HPP_
#define MAP_TRIPLE_FWD_HPP_
#include "boost/mpl/empty.hpp"
#include "owlcpp/rdf/map_triple_config.hpp"

namespace owlcpp{

/**@brief Store, index, and search RDF triples
*******************************************************************************/
template<
   class Config = map_triple_detail::index_config_default,
   bool Empty = boost::mpl::empty<Config>::value
>class Map_triple;


}//namespace owlcpp
#endif /* MAP_TRIPLE_FWD_HPP_ */
