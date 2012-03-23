/** @file "/owlcpp/include/owlcpp/rdf/store_node_blank_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_NODE_BLANK_CRTPB_HPP_
#define STORE_NODE_BLANK_CRTPB_HPP_
#include <string>
#include "owlcpp/doc_id.hpp"

namespace owlcpp{

/** CRTP base for storing blank nodes in RDF triple stores
*******************************************************************************/
template<class T> struct Store_node_blank_crtpb {

   /**@brief Insert blank node
    @param did document ID
    @param name blank node name (MUST be unique within the document)
    @return node ID
   */
   Node_id insert_blank_node(const Doc_id did, std::string const& name) {
      T& self = static_cast<T&>(*this);
      return self.nodes().insert_blank(did, name);
   }

};

}//namespace owlcpp
#endif /* STORE_NODE_BLANK_CRTPB_HPP_ */
