/** @file "/owlcpp/include/owlcpp/rdf/node_store_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_STORE_OWL_HPP_
#define NODE_STORE_OWL_HPP_
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{

/**@brief Store RDF nodes including standard OWL/RDF nodes
*******************************************************************************/
class OWLCPP_RDF_DECL Node_store_owl {
public:
   typedef Node_map::iterator iterator;
   struct Err : public base_exception {};
   Node_store_owl();
   std::size_t size() const {return store_.size();}
   Node const& operator[](const Node_id id) const {return store_[id];}
   Node const& at(const Node_id id) const {return store_.at(id);}
   Node_id const* find(Node const& node) const {return store_.find(node);}
   void remove(const Node_id id) {store_.remove(id);}
   void remove(Node const& node) {store_.remove(node);}
   iterator begin() const {return store_.begin();}
   iterator end() const {return store_.end();}

   /**@brief insert new node into the store
    @param node node to be stored
    @return node ID
    @throw Err if node namespace is one of the standard OWL/RDF and the value is unknown
   */
   Node_id insert(Node const& node);

private:
   Node_map store_;
};

}//namespace owlcpp
#endif /* NODE_STORE_OWL_HPP_ */
