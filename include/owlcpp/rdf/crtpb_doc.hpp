/** @file "/owlcpp/include/owlcpp/rdf/crtpb_doc.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef CRTPB_DOC_HPP_
#define CRTPB_DOC_HPP_
#include <string>

#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/** Enable interaction between a map of ontology documents and
a map of IRI nodes.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Crtpb_doc {
   typedef typename Map_traits<Super>::map_node_t map_node_t;
   typedef typename Map_traits<Super>::map_doc_t map_doc_t;

   map_node_t const& node() const {
      return static_cast<Super const&>(*this).nodes();
   }

   map_node_t& ns() {
      return static_cast<Super&>(*this).node_;
   }

public:

};

}//namespace owlcpp
#endif /* CRTPB_DOC_HPP_ */
