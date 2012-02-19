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

   template<class T> Node_map_std(T const& t)
   : iris_(),
     ns_id_next_(t(iris_)),
     nodes_(),
     node_id_next_(t(nodes_))
     {}

public:
   typedef Node_map_base::ns_range ns_range;
   typedef Node_map_base::node_range node_range;

   /**
    @tparam T SHOULD be callable with IRI map and node map that inserts
    standard namespace IRIs and standard nodes returning next available
    namespace ID and node ID correspondingly.

    @code
   struct Inserter {
   Ns_id operator()(Iri_map_base& map) const;
   Node_id operator()(Node_map_base& map) const;
   };
   Node_map_std const& nms = Node_map_std::get(Inserter());
   @endcode

   */
   template<class T> static Node_map_std const& get(T const& t) {
      static const Node_map_std map((t));
      return map;
   }

   Ns_id ns_id_next() const {return ns_id_next_;}
   bool have(const Ns_id iid) const {return iid < ns_id_next_;}
   Ns_id const* find_iri(std::string const& iri) const {return iris_.find_iri(iri);}
   Ns_id const* find_prefix(std::string const& pref) const {return iris_.find_prefix(pref);}
   std::string operator[](const Ns_id iid) const {return iris_[iid];}
   std::string at(const Ns_id iid) const {return iris_.at(iid);}
   std::string prefix(const Ns_id iid) const {return iris_.prefix(iid);}

   Node_id node_id_next() const {return node_id_next_;}
   bool have(const Node_id nid) const {return nid < node_id_next_;}
   Node const& operator[](const Node_id nid) const {return nodes_[nid];}
   Node const& at(const Node_id nid) const {return nodes_.at(nid);}
   ns_range find(const Ns_id iid) const {return nodes_.find(iid);}
   node_range find(Node const& node) const {return nodes_.find(node);}

private:
   Iri_map_base iris_;
   const Ns_id ns_id_next_;
   Node_map_base nodes_;
   const Node_id node_id_next_;

};

}//namespace owlcpp
#endif /* NODE_MAP_STD_HPP_ */
