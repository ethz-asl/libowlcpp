/** @file "/owlcpp/include/owlcpp/rdf/detail/map_traits.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RDF_MAP_TRAITS_HPP_
#define RDF_MAP_TRAITS_HPP_

#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/rdf/map_triple_fwd.hpp"

namespace owlcpp{
class Map_ns;
class Map_node;
class Triple_store;
class Map_std;
class Map_doc;
class Node;
struct Doc_meta;
class Ns_id;
class Node_id;
class Doc_id;

namespace detail{


/**@brief
*******************************************************************************/
template<> struct Map_traits<Triple_store> {
   typedef Map_ns map_ns_type;
   typedef Map_node map_node_type;
   typedef Map_std map_std_type;
   typedef Map_doc map_doc_type;
   typedef Node node_type;
   typedef Doc_meta doc_type;
   typedef Ns_id ns_id_type;
   typedef Node_id node_id_type;
   typedef Doc_id doc_id_type;
   typedef Map_triple<> map_triple_type;
};

}//namespace detail
}//namespace owlcpp
#endif /* RDF_MAP_TRAITS_HPP_ */
