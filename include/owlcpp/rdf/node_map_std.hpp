/** @file "/owlcpp/include/owlcpp/rdf/node_map_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_STD_HPP_
#define NODE_MAP_STD_HPP_
#include "owlcpp/rdf/iri_map_base.hpp"
#include "owlcpp/rdf/node_map_base.hpp"

namespace owlcpp{

/**@brief immutable static set of IRIs and nodes
*******************************************************************************/
class Node_map_std {
   Node_map_std();
   Node_map_std(Node_map_std const&);
   Node_map_std& operator=(Node_map_std const&);

   template<class T> Node_map_std(T const&)
   : iris_(),
     max_ns_id_(T::fill(iris_)),
     nodes_(),
     max_node_id_(T::fill(nodes_))
     {}

public:
   template<class T> static Node_map_std const& get(T const& t) {
      static const Node_map_std map((t));
      return map;
   }

private:
   Iri_map_base iris_;
   const Ns_id max_ns_id_;
   Node_map_base nodes_;
   Node_id max_node_id_;

};

}//namespace owlcpp
#endif /* NODE_MAP_STD_HPP_ */
