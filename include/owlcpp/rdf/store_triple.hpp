/** @file "/owlcpp/include/owlcpp/rdf/store_triple.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_TRIPLE_HPP_
#define STORE_TRIPLE_HPP_
#include "owlcpp/rdf/detail/map_ns_base.hpp"
#include "owlcpp/rdf/detail/map_node_base.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/map_doc.hpp"
#include "owlcpp/rdf/map_node_std.hpp"
#include "owlcpp/rdf/map_triple.hpp"
#include "owlcpp/rdf/crtpb_ns_std.hpp"
#include "owlcpp/rdf/crtpb_node_std.hpp"
#include "owlcpp/rdf/crtpb_ns_node_iri.hpp"
#include "owlcpp/rdf/nodes_std.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Store_triple :
public Crtpb_ns_node_iri<Store_triple>,
public Crtpb_node_std<Store_triple>,
public Crtpb_ns_std<Store_triple>
{
//   friend class Crtpb_ns_node_iri<Store_triple>; //public access is enough
   friend class Crtpb_ns_std<Store_triple>;
   friend class Crtpb_node_std<Store_triple>;

   typedef typename Map_traits<Store_triple>::map_ns_t map_ns_t;
   typedef typename Map_traits<Store_triple>::map_node_t map_node_t;
   typedef typename Map_traits<Store_triple>::map_node_std_t map_node_std_t;
   typedef typename Map_traits<Store_triple>::map_doc_t map_doc_t;
   typedef typename Map_traits<Store_triple>::map_triple_t map_triple_t;

public:
   struct Err : public Rdf_err {};

   Store_triple()
   : snode_(Map_node_std::get(Nodes_owl())),
     ns_(snode_.ns_id_next()),
     node_(snode_.node_id_next()),
     doc_(),
     triple_()
   {}

   template<class Nodes> explicit Store_triple(Nodes const& nodes)
   : snode_(Map_node_std::get(nodes)),
     ns_(snode_.ns_id_next()),
     node_(snode_.node_id_next()),
     doc_(),
     triple_()
   {}

   map_ns_t const& namespaces() const {return ns_;}
   map_node_t const& nodes() const {return node_;}
   map_doc_t const& docs() const {return doc_;}
   map_triple_t const& triples() const {return triple_;}

   //bring in overloaded methods
   using Crtpb_node_std<Store_triple>::operator[];
   using Crtpb_ns_std<Store_triple>::operator[];
   using Crtpb_node_std<Store_triple>::at;
   using Crtpb_ns_std<Store_triple>::at;

   using Crtpb_ns_node_iri<Store_triple>::insert_node_iri;
   using Crtpb_node_std<Store_triple>::insert_node_iri;

private:
   map_node_std_t const& snode_;
   map_ns_t ns_;
   map_node_t node_;
   map_doc_t doc_;
   map_triple_t triple_;

};

}//namespace owlcpp
#endif /* STORE_TRIPLE_HPP_ */
