/** @file "/owlcpp/include/owlcpp/rdf/detail/map_traits.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_TRAITS_HPP_
#define MAP_TRAITS_HPP_

namespace owlcpp{
class Map_ns;
class Map_node;
class Triple_store;
class Map_std;
class Map_doc;
class Node;
class Doc_meta;
class Ns_id;
class Node_id;
class Doc_id;
template<bool,bool,bool,bool> class Map_triple;

namespace detail{
/**@brief 
*******************************************************************************/
template<class M> struct Map_traits;


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
   > map_triple_type;

};

}//namespace detail
}//namespace owlcpp
#endif /* MAP_TRAITS_HPP_ */
