/** @file "/owlcpp/include/owlcpp/rdf/store_node_literal_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_NODE_LITERAL_CRTPB_HPP_
#define STORE_NODE_LITERAL_CRTPB_HPP_
#include <string>
#include "owlcpp/node_id.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/** CRTP base for storing literal nodes in RDF triple stores
*******************************************************************************/
template<class T> struct Store_node_literal_crtpb {

   /**@brief if not already present, store literal node
    @param val literal node string value
    @param datatype datatype IRI
    @param lang string value language
    @return node ID
   */
   Node_id insert_lit_node(
            std::string const& val,
            std::string const& datatype = "",
            std::string const& lang = ""
   ) {
      T& self = static_cast<T&>(*this);
      const Node_id dti =
               datatype.empty() ?
                        terms::T_empty_::id() :
                        self.insert_iri_node(datatype);

      return self.nodes().insert_literal(val, dti, lang);
   }

};

}//namespace owlcpp
#endif /* STORE_NODE_LITERAL_CRTPB_HPP_ */
