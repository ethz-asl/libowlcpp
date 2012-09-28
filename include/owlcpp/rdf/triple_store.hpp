/** @file "/owlcpp/include/owlcpp/rdf/triple_store.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_HPP_
#define TRIPLE_STORE_HPP_
#include "owlcpp/rdf/map_ns.hpp"
#include "owlcpp/rdf/detail/map_node_base.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/map_doc.hpp"
#include "owlcpp/rdf/map_std.hpp"
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

   typedef detail::Map_traits<Triple_store> traits;
   typedef typename traits::map_ns_type map_ns_type;
   typedef typename traits::map_node_type map_node_type;
   typedef typename traits::map_std_type map_std_type;
   typedef typename traits::map_doc_type map_doc_type;
   typedef typename traits::map_triple_type map_triple_type;

public:
   struct Err : public Rdf_err {};

   typedef map_doc_type::iri_range doc_iri_range;
   typedef map_doc_type::version_range doc_version_range;

   Triple_store()
   : map_std_(Map_node_std::get(Nodes_owl())),
     map_ns_(map_std_.ns_id_next()),
     map_node_(map_std_.node_id_next()),
     map_doc_(),
     map_triple_()
   {}

   template<class Nodes> explicit Triple_store(Nodes const& nodes)
   : map_std_(Map_node_std::get(nodes)),
     map_ns_(map_std_.ns_id_next()),
     map_node_(map_std_.node_id_next()),
     map_doc_(),
     map_triple_()
   {}

   map_ns_type const& namespaces() const {return map_ns_;}
   map_node_type const& nodes() const {return map_node_;}
   map_doc_type const& docs() const {return map_doc_;}
   map_triple_type const& triples() const {return map_triple_;}

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
   map_std_type const& map_std_;
   map_ns_type map_ns_;
   map_node_type map_node_;
   map_doc_type map_doc_;
   map_triple_type map_triple_;

};

}//namespace owlcpp
#endif /* TRIPLE_STORE_HPP_ */
