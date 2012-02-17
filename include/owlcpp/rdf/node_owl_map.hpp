/** @file "/owlcpp/include/owlcpp/rdf/node_owl_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_OWL_MAP_HPP_
#define NODE_OWL_MAP_HPP_
#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class OWLCPP_RDF_DECL Node_owl_map {
   Node_owl_map();
   Node_owl_map(Node_owl_map const&);
public:
   typedef Node_map_base::const_iterator const_iterator;
   typedef Node_map_base::iterator iterator;
   typedef Node_map_base::range range;
   typedef Node_map_base::node_iterator node_iterator;
   typedef Node_map_base::node_range node_range;
   typedef Node_map_base::ns_iterator ns_iterator;
   typedef Node_map_base::ns_range ns_range;

   static Node_owl_map const& get() {
      static const Node_owl_map map;
      return map;
   }

   bool is_owl(const Node_id nid) const {return nid <= max_;}
   bool is_owl(const Ns_id iid) const {return is_owl(iid);}
   Node_id max_id() const {return max_;}

   std::size_t size() const {return map_.size();}
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}
   Node const& operator[](const Node_id id) const {return map_[id];}
   Node const& at(const Node_id id) const {return map_.at(id);}
   ns_range find(const Ns_id iid) const {return map_.find(iid);}
   node_range find(Node const& node) const {return map_.find(node);}

private:
   Node_map_base map_;
   Node_id max_;
};

/**@brief
*******************************************************************************/
struct Owl_nodes {
   Node_id max_id() const {return Node_owl_map::get().max_id();}
   bool have(const Node_id nid) const {return Node_owl_map::get().is_owl(nid);}
   bool have(const Ns_id iid) const {return Node_owl_map::get().is_owl(iid);}
   Node const& operator[](const Node_id id) const {return Node_owl_map::get()[id];}
   Node const& at(const Node_id id) const {return Node_owl_map::get().at(id);}
   Node_map_base::ns_range find(const Ns_id iid) const {return Node_owl_map::get().find(iid);}
   Node_map_base::node_range find(Node const& node) const {return Node_owl_map::get().find(node);}
};

}//namespace owlcpp
#endif /* NODE_OWL_MAP_HPP_ */
