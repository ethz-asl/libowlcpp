/** @file "/owlcpp/include/owlcpp/rdf/triple_store.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_HPP_
#define TRIPLE_STORE_HPP_
#include "owlcpp/rdf/crtpb_ns_node_iri.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"
#include "owlcpp/rdf/map_doc_crtpb.hpp"
#include "owlcpp/rdf/map_doc.hpp"
#include "owlcpp/rdf/map_node_blank_crtpb.hpp"
#include "owlcpp/rdf/map_node_literal_crtpb.hpp"
#include "owlcpp/rdf/map_node.hpp"
#include "owlcpp/rdf/map_ns.hpp"
#include "owlcpp/rdf/map_std_node_crtpb.hpp"
#include "owlcpp/rdf/map_std_ns_crtpb.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/rdf/map_triple_crtpb.hpp"
#include "owlcpp/rdf/map_triple.hpp"
#include "owlcpp/rdf/nodes_std.hpp"

namespace owlcpp{

/**@brief Store namespace IRIs, RDF nodes, document infos, and RDF triples

@cond DEV
@decision define interfaces through several CRTP base classes
to be able to reuse them elsewhere in the project
@endcond
*******************************************************************************/
class Triple_store :
public Map_std_ns_crtpb<Triple_store>,
public Map_std_node_crtpb<Triple_store>,
public Crtpb_ns_node_iri<Triple_store>,
public Map_doc_crtpb<Triple_store>,
public Map_node_literal_crtpb<Triple_store>,
public Map_node_blank_crtpb<Triple_store>,
public Map_triple_crtpb<Triple_store>
{
   friend class Map_std_ns_crtpb<Triple_store>;
   friend class Map_std_node_crtpb<Triple_store>;
   friend class Map_node_literal_crtpb<Triple_store>;
   friend class Map_node_blank_crtpb<Triple_store>;
   friend class Map_doc_crtpb<Triple_store>;
   friend class Map_triple_crtpb<Triple_store>;

   typedef detail::Map_traits<Triple_store> traits;

public:
   typedef traits::map_std_type map_std_type;
   typedef traits::map_ns_type map_ns_type;
   typedef traits::map_node_type map_node_type;
   typedef traits::map_doc_type map_doc_type;
   typedef traits::map_triple_type map_triple_type;

   struct Err : public Rdf_err {};

   Triple_store()
   : map_std_(map_std_type::get(Nodes_owl()))
   {}

   template<class Nodes_std> explicit Triple_store(Nodes_std const& nodes_std)
   : map_std_(map_std_type::get(nodes_std))
   {}

   map_std_type const& map_std() const {return map_std_;}
   map_ns_type const& map_ns() const {return map_ns_;}
   map_node_type const& map_node() const {return map_node_;}
   map_doc_type const& map_doc() const {return map_doc_;}
   map_triple_type const& map_triple() const {return map_triple_;}

   //disambiguate overloaded methods
   using Map_std_ns_crtpb<Triple_store>::operator[];
   using Map_std_node_crtpb<Triple_store>::operator[];
   using Map_doc_crtpb<Triple_store>::operator[];
   using Map_std_ns_crtpb<Triple_store>::at;
   using Map_std_node_crtpb<Triple_store>::at;
   using Map_doc_crtpb<Triple_store>::at;

   using Map_std_node_crtpb<Triple_store>::insert_node_iri;
   using Crtpb_ns_node_iri<Triple_store>::insert_node_iri;

   using Map_std_node_crtpb<Triple_store>::find_node_iri;
   using Crtpb_ns_node_iri<Triple_store>::find_node_iri;

   using Map_std_ns_crtpb<Triple_store>::find;
   using Map_std_node_crtpb<Triple_store>::find;
   using Map_doc_crtpb<Triple_store>::find;

   using Map_std_ns_crtpb<Triple_store>::insert;
   using Map_triple_crtpb<Triple_store>::insert;

private:
   map_std_type const& map_std_;
   map_ns_type map_ns_;
   map_node_type map_node_;
   map_doc_type map_doc_;
   map_triple_type map_triple_;

};

/**Example of searching triple store
@example print_classes.cpp
*/

}//namespace owlcpp
#endif /* TRIPLE_STORE_HPP_ */
