/** @file "/owlcpp/include/owlcpp/rdf/map_node_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_STD_HPP_
#define MAP_NODE_STD_HPP_
#include "boost/range.hpp"
#include "boost/range/algorithm/max_element.hpp"
#include "boost/utility.hpp"

#include "owlcpp/rdf/detail/map_ns_base.hpp"
#include "owlcpp/rdf/detail/map_node_base.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**@brief immutable static set of IRIs and nodes
@details Contains at least blank and empty (literal) namespaces and empty node
*******************************************************************************/
class Map_node_std : boost::noncopyable {

   template<class Inserter> explicit Map_node_std(Inserter const& ins)
   : ns_(Ns_id(0)), ns_id_next_(0), node_(Node_id(0)), node_id_next_(0)
   {
      using namespace owlcpp::terms;
      ns_.insert(N_empty::id(), N_empty::iri());
      ns_.set_prefix(N_empty::id(), N_empty::prefix());
      ns_.insert(N_blank::id(), N_blank::iri());
      ns_.set_prefix(N_blank::id(), N_blank::prefix());
      node_.insert_iri_tag(terms::T_empty_());

      ins(ns_);
      ins(node_);
      detail::Map_ns_base::const_iterator i = boost::max_element(ns_);
      ns_id_next_ = Ns_id((*i)() + 1);
      detail::Map_node_base::const_iterator j = boost::max_element(node_);
      node_id_next_ = Node_id((*j)() + 1);
   }

   typedef detail::Map_ns_base map_ns;
   typedef detail::Map_node_base map_node;
public:

   /**
    @tparam T SHOULD be callable with IRI map and node map inserting
    standard namespace IRIs and standard nodes.

    @code
   struct Inserter {
   void operator()(Iri_map_base& map) const;
   void operator()(Node_map_base& map) const;
   };
   Node_map_std const& nms = Node_map_std::get(Inserter());
   @endcode

   */
   template<class T> static Map_node_std const& get(T const& t) {
      static const Map_node_std map((t));
      return map;
   }

private:
   detail::Map_ns_base ns_;
   Ns_id ns_id_next_;
   detail::Map_node_base node_;
   Node_id node_id_next_;
};

}//namespace owlcpp
#endif /* MAP_NODE_STD_HPP_ */
