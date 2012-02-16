/** @file "/owlcpp/lib/rdf/node_owl_map.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/node_owl_map.hpp"

#include "boost/mpl/for_each.hpp"

#include "node_tag_vector_system.hpp"
#include "node_tag_vector_owl.hpp"

namespace owlcpp { namespace{
class Node_tag_inserter {
public:
   explicit Node_tag_inserter(Node_map_base& map, Node_id& id)
   : map_(&map), max_(id) {}

   template<class T> void operator()(T const&) const {
      map_->insert_tag(T());
      if( T::id() > max_) max_ = T::id();
   }

private:
   mutable Node_map_base* map_;
   mutable Node_id& max_;
};
}//namespace anonymous

/*
*******************************************************************************/
Node_owl_map::Node_owl_map()
: map_(), max_(0)
{
   Node_tag_inserter nti(map_, max_);
   boost::mpl::for_each<terms::mpl_vector_terms_system_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(nti);
   boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(nti);
}

}//namespace owlcpp
