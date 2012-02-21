/** @file "/owlcpp/include/owlcpp/rdf/node_map_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_STD_HPP_
#define NODE_MAP_STD_HPP_
#include "boost/range.hpp"
#include "boost/range/algorithm/max_element.hpp"
#include "owlcpp/rdf/iri_map_base.hpp"
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**@brief immutable static set of IRIs and nodes
@details Contains at least blank and empty (litaral) namespaces and empty node
*******************************************************************************/
class Node_map_std {
   Node_map_std();
   Node_map_std(Node_map_std const&);
   Node_map_std& operator=(Node_map_std const&);

   template<class T> explicit Node_map_std(T const& t)
   : iris_(), ns_id_next_(0), nodes_(), node_id_next_(0)
     {
      iris_.insert_tag(terms::N_empty());
      iris_.insert_tag(terms::N_blank());
      nodes_.insert_tag(terms::T_empty_());

      t(iris_);
      t(nodes_);
      Iri_map_base::const_iterator i = boost::max_element(iris_);
      if( i != iris_.end() ) ns_id_next_ = Ns_id((*i)() + 1);
      Node_map_base::const_iterator j = boost::max_element(nodes_);
      if( j != nodes_.end() ) node_id_next_ = Node_id((*j)() + 1);
     }

public:
   typedef Node_map_base::ns_range ns_range;
   typedef Node_map_base::node_range node_range;

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
   template<class T> static Node_map_std const& get(T const& t) {
      static const Node_map_std map((t));
      return map;
   }

   /**
    @param iid namespace IRI ID
    @return true if nodes should not be added to this namespace by user;
    e.g., in case of RDF or OWL namespaces.
    That is, all nodes from this namespace should reside in this map.
   */
   bool is_constant(const Ns_id iid) const {
      return iid > terms::N_empty::id() && iid > terms::N_blank::id() && iid < ns_id_next_;
   }

   Ns_id ns_id_next() const {return ns_id_next_;}
   Ns_id const* find_iri(std::string const& iri) const {return iris_.find_iri(iri);}
   Ns_id const* find_prefix(std::string const& pref) const {return iris_.find_prefix(pref);}
   std::string operator[](const Ns_id iid) const {return iris_[iid];}
   std::string at(const Ns_id iid) const {return iris_.at(iid);}
   std::string prefix(const Ns_id iid) const {return iris_.prefix(iid);}

   Node_id node_id_next() const {return node_id_next_;}
   Node const& operator[](const Node_id nid) const {return nodes_[nid];}
   Node const& at(const Node_id nid) const {return nodes_.at(nid);}
   ns_range find(const Ns_id iid) const {return nodes_.find(iid);}
   node_range find(Node const& node) const {return nodes_.find(node);}

private:
   Iri_map_base iris_;
   Ns_id ns_id_next_;
   Node_map_base nodes_;
   Node_id node_id_next_;
};

}//namespace owlcpp
#endif /* NODE_MAP_STD_HPP_ */
