/** @file "/owlcpp/include/owlcpp/rdf/nodes_blank_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODES_BLANK_CRTPB_HPP_
#define NODES_BLANK_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/node_map_base.hpp"
#include "owlcpp/doc_id.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
template<class Super> class Nodes_blank_crtpb {
   typedef boost::unordered_map<Node_id,Doc_id> docs_t;
   typedef docs_t::const_iterator doc_iter_t;

public:
   /**@brief Insert blank node
    @param did document ID
    @param name blank node name (MUST be unique within the document)
    @return node ID
   */
   Node_id insert_blank(const Doc_id did, std::string const& name) {
      Node_id const*const nid1 = find_blank(did, name);
      if( nid1 ) return *nid1;
      Super& self = static_cast<Super&>(*this);
      const Node_id nid2 = self.insert_new(terms::N_blank::id(), name);
      docs_.emplace(nid2, did);
      return nid2;
   }

   Node_id const* find_blank(const Doc_id did, std::string const& name) {
      return find_blank(did, Node(terms::N_blank::id(), name));
   }

   /**
    @param id ID of a blank node
    @return ID of the document to which the blank node belongs
    @throw Rdf_err if @b id is not for a blank node
   */
   Doc_id blank_node_doc(const Node_id id) const {
      //BOOST_ASSERT(docs_.find(id) != docs_.end());
      typedef typename Super::Err Err;
      const doc_iter_t i = docs_.find(id);
      if( i == docs_.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << typename Err::msg_t("document unknown")
               << typename Err::int1_t(id())
      );
      return i->second;
   }

protected:
   void remove_blank(const Node_id nid) {
      BOOST_ASSERT(docs_.find(nid) != docs_.end());
      docs_.erase(nid);
   }

private:
   docs_t docs_;

   Node_id const* find_blank(const Doc_id did, Node const& node) {
      Node_map_base const& nmb = static_cast<Super const&>(*this).node_map();
      BOOST_FOREACH(Node_id const& id, nmb.find(node)) {
         const doc_iter_t i = docs_.find(id);
         BOOST_ASSERT(i != docs_.end());
         if( i->second == did ) return &id;
      }
      return 0;
   }
};

}//namespace owlcpp
#endif /* NODES_BLANK_CRTPB_HPP_ */
