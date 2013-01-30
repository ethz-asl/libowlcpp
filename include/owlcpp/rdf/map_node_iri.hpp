/** @file "/owlcpp/include/owlcpp/rdf/map_node_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012-3
*******************************************************************************/
#ifndef MAP_NODE_IRI_HPP_
#define MAP_NODE_IRI_HPP_
#include <string>
#include <vector>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include "boost/concept_check.hpp"

#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/detail/map_id_object.hpp"

#include "owlcpp/detail/iterator_member_pair.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**@brief Store RDF IRI nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Map_node_iri {
public:
   typedef Node_iri node_type;
   typedef Node_id id_type;
private:
   typedef detail::Map_id_object<node_type, id_type> map_t;

public:
   typedef Iterator_member_pair<map_t::const_iterator, Node_id const, 2> iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   Map_node_iri(const Node_id id0 = detail::min_node_id())
   : map_(id0)
   {}

   std::size_t size() const { return map_.size(); }
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}
   bool empty() const {return map_.empty();}

   Node_iri const& operator[](const Node_id id) const {return map_[id];}
   Node_iri const& at(const Node_id id) const {return map_.at(id);}
   Node_iri const* find(const Node_id id) const {return map_.find(id);}
   Node_id const* find(Node_iri const& node) const {return map_.find(node);}

   Node_id const* find_iri(const Ns_id ns, std::string const& val) const {
      return map_.find(Node_iri(ns,val));
   }

   Node_id insert_iri(const Ns_id ns, std::string const& val) {
      return map_.insert(Node_iri(ns, val));
   }

   Node_id insert(Node_iri const& node) {return map_.insert(node);}
   void remove(const Node_id id) {map_.remove(id);}
   void clear() {map_.clear();}

private:
   map_t map_;
};

}//namespace owlcpp
#endif /* MAP_NODE_IRI_HPP_ */
