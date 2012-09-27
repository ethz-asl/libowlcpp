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
#include "owlcpp/rdf/crtpb_doc.hpp"
#include "owlcpp/rdf/nodes_std.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Triple_store :
public Crtpb_ns_node_iri<Triple_store>,
public Crtpb_node_std<Triple_store>,
public Crtpb_doc<Triple_store>,
public Crtpb_ns_std<Triple_store>
{
//   friend class Crtpb_ns_node_iri<Triple_store>; //public access is enough
   friend class Crtpb_ns_std<Triple_store>;
   friend class Crtpb_node_std<Triple_store>;
   friend class Crtpb_doc<Triple_store>;

   typedef typename Map_traits<Triple_store>::map_ns_t map_ns_t;
   typedef typename Map_traits<Triple_store>::map_node_t map_node_t;
   typedef typename Map_traits<Triple_store>::map_node_std_t map_node_std_t;
   typedef typename Map_traits<Triple_store>::map_doc_t map_doc_t;
   typedef typename Map_traits<Triple_store>::map_triple_t map_triple_t;

public:
   struct Err : public Rdf_err {};

   typedef map_doc_t::iri_range doc_iri_range;
   typedef map_doc_t::version_range doc_version_range;

   Triple_store()
   : snode_(Map_node_std::get(Nodes_owl())),
     ns_(snode_.ns_id_next()),
     node_(snode_.node_id_next()),
     doc_(),
     triple_()
   {}

   template<class Nodes> explicit Triple_store(Nodes const& nodes)
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
   using Crtpb_node_std<Triple_store>::operator[];
   using Crtpb_ns_std<Triple_store>::operator[];
   using Crtpb_doc<Triple_store>::operator[];
   using Crtpb_node_std<Triple_store>::at;
   using Crtpb_ns_std<Triple_store>::at;
   using Crtpb_doc<Triple_store>::at;

   using Crtpb_ns_node_iri<Triple_store>::insert_node_iri;
   using Crtpb_node_std<Triple_store>::insert_node_iri;

   using Crtpb_ns_std<Triple_store>::valid;
   using Crtpb_node_std<Triple_store>::valid;

private:
   map_node_std_t const& snode_;
   map_ns_t ns_;
   map_node_t node_;
   map_doc_t doc_;
   map_triple_t triple_;

};

}//namespace owlcpp
#endif /* STORE_TRIPLE_HPP_ */
