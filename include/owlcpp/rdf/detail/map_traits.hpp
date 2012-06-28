/** @file "/owlcpp/include/owlcpp/rdf/detail/map_traits.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_TRAITS_HPP_
#define MAP_TRAITS_HPP_

namespace owlcpp{ namespace detail{
class Map_ns_base;
class Map_node_base;
}//namespace detail

/**@brief 
*******************************************************************************/
template<class M> struct Map_traits;

class Store_triple;
class Map_node_std;
class Map_doc;
template<bool,bool,bool,bool> class Map_triple;

/**@brief
*******************************************************************************/
template<> struct Map_traits<Store_triple> {
   typedef detail::Map_ns_base map_ns_t;
   typedef detail::Map_node_base map_node_t;
   typedef Map_node_std map_node_std_t;
   typedef Map_doc map_doc_t;

   typedef Map_triple<
#ifdef OWLCPP_RDF_INDEX_SUBJECT
            OWLCPP_RDF_INDEX_SUBJECT
#else
            1
#endif
            ,
#ifdef OWLCPP_RDF_INDEX_PREDICATE
            OWLCPP_RDF_INDEX_PREDICATE
#else
            0
#endif
            ,
#ifdef OWLCPP_RDF_INDEX_OBJECT
            OWLCPP_RDF_INDEX_OBJECT
#else
            0
#endif
            ,
#ifdef OWLCPP_RDF_INDEX_DOCUMENT
            OWLCPP_RDF_INDEX_DOCUMENT
#else
            0
#endif
   > map_triple_t;

};

}//namespace owlcpp
#endif /* MAP_TRAITS_HPP_ */
