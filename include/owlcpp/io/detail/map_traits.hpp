/** @file "/owlcpp/include/owlcpp/io/detail/map_traits.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef IO_MAP_TRAITS_HPP_
#define IO_MAP_TRAITS_HPP_

#include "owlcpp/detail/map_traits.hpp"

namespace owlcpp{
class Map_std;
class Map_ns;
class Map_node_iri;
class Map_doc;
class Node_iri;
struct Doc_meta;
class Ns_id;
class Node_id;
class Doc_id;
class Catalog;

namespace detail{


/**@brief
*******************************************************************************/
template<> struct Map_traits<Catalog> {
   typedef Map_std map_std_type;
   typedef Map_ns map_ns_type;
   typedef Map_node_iri map_node_type;
   typedef Map_doc map_doc_type;
   typedef Node_iri node_type;
   typedef Doc_meta doc_type;
   typedef Ns_id ns_id_type;
   typedef Node_id node_id_type;
   typedef Doc_id doc_id_type;
};

}//namespace detail
}//namespace owlcpp
#endif /* IO_MAP_TRAITS_HPP_ */
